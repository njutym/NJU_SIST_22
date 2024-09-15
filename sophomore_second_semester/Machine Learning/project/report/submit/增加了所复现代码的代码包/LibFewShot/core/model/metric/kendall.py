# -*- coding: utf-8 -*-
"""
@inproceedings{zheng2023diffkendall,
  author    = {Kaipeng Zheng and 
               Huishuai Zhang and 
               Weiran Huang},
  title     = {DiffKendall: A Novel Approach for Few-Shot Learning 
               with Differentiable Kendall's Rank Correlation},
  booktitle = {Thirty-seventh Conference on Neural Information Processing Systems},
  year      = {2023},
  url       = {https://openreview.net/forum?id=SVUQX1W7RL}
}

Adapted from https://github.com/WenbinLee/Kendall.
"""

import torch
import torch.nn.functional as F
from torch import nn
from core.utils import accuracy
from core.model.metric.metric_model import MetricModel
import random
from itertools import combinations    

class KendallLayer(nn.Module):
    def __init__(self, mode='diffkendall', beta=1, T=0.0125):
        super(KendallLayer, self).__init__()
        self.mode = mode
        self.T = T 
        self.beta = beta

    def change_mode(self, mode='diffkendall'):
        self.mode = mode

    def forward(
        self, 
        query_feat,
        support_feat,
        way_num,
        shot_num,
        query_num,
    ):
        if self.mode == 'cosine':
            t , wq , c , h , w = query_feat.size()
            _ , ws , _ , _ , _ = support_feat.size()
            # support : task , way * shot , channel , h , w
            # query : task , way * query , channel , h , w
            support_feat = F.adaptive_avg_pool2d(support_feat, [1, 1]).squeeze(dim = (3 , 4))
            query_feat = F.adaptive_avg_pool2d(query_feat, [1, 1]).squeeze(dim = (3 , 4))
            support_feat = support_feat.view(t , way_num , shot_num , c).mean(dim = 2)
            # support : task , way , channel
            # query : task , way * query , channel
            query_feat = query_feat.view(t * wq , c)
            support_feat = support_feat.view(t * way_num ,c)
            output = torch.mm(F.normalize(query_feat, dim=-1), F.normalize(support_feat, dim=-1).T)
            return output
        if self.mode == 'kendall':
            t , wq , c , h , w = query_feat.size()
            _ , ws , _ , _ , _ = support_feat.size()
            # support : task , way * shot , channel , h , w
            # query : task , way * query , channel , h , w
            support_feat = F.adaptive_avg_pool2d(support_feat, [1, 1]).squeeze(dim = (3 , 4))
            query_feat = F.adaptive_avg_pool2d(query_feat, [1, 1]).squeeze(dim = (3 , 4))
            support_feat = support_feat.view(t , way_num , shot_num , c).mean(dim = 2)
            # support : task , way , channel
            # query : task , way * query , channel
            pair_num = int(c * (c - 1) // 2)
            c_pair = random.sample(sorted(combinations(list(range(c)), 2)), pair_num)
            query_feat = query_feat.view(t * wq , c)
            support_feat = support_feat.view(t * way_num ,c)
            query_feat = query_feat[: , c_pair].diff().sign().squeeze()
            support_feat = support_feat[: , c_pair].diff().sign().squeeze()
            output = torch.mm(query_feat , support_feat.T)
            output = output / pair_num
            return output
        if self.mode == 'diffkendall':
            t , wq , c , h , w = query_feat.size()
            _ , ws , _ , _ , _ = support_feat.size()
            # support : task , way * shot , channel , h , w
            # query : task , way * query , channel , h , w
            support_feat = F.adaptive_avg_pool2d(support_feat, [1, 1]).squeeze(dim = (3 , 4))
            query_feat = F.adaptive_avg_pool2d(query_feat, [1, 1]).squeeze(dim = (3 , 4))
            support_feat = support_feat.view(t , way_num , shot_num , c).mean(dim = 2)
            # support : task , way , channel
            # query : task , way * query , channel
            pair_num = int(c * (c - 1) // 2)
            c_pair = random.sample(sorted(combinations(list(range(c)), 2)), pair_num)
            query_feat = query_feat.view(t * wq , c)
            support_feat = support_feat.view(t * way_num ,c)
            query_feat = query_feat[: , c_pair].diff().squeeze()
            support_feat = support_feat[: , c_pair].diff().squeeze()
            score = support_feat.repeat([query_feat.shape[0], 1, 1]) * query_feat.unsqueeze(1).repeat([1, support_feat.shape[0], 1])
            score = 1 / (1 + (-score * self.beta).exp())
            score = 2 * score - 1
            score = score.mean(dim=-1)
            score = score / self.T
            return score
        
        # default
        else:
            raise ValueError('Unknown mode')

class Kendall(MetricModel):
    def __init__(self, train_type='diffkendall', test_type='diffkendall', beta=1, T=0.0125 ,**kwargs):
        super(Kendall, self).__init__(**kwargs)
        self.kendall_layer = KendallLayer(train_type,beta,T)
        self.loss_func = nn.CrossEntropyLoss()
        self.train_type = train_type
        self.test_type = test_type

    def set_forward(self, batch):
        """

        :param batch:
        :return:
        """
        image, global_target = batch
        image = image.to(self.device)
        episode_size = image.size(0) // (
            self.way_num * (self.shot_num + self.query_num)
        )
        feat = self.emb_func(image)
        support_feat, query_feat, support_target, query_target = self.split_by_episode(
            feat, mode=2
        )
        self.kendall_layer.change_mode(self.test_type)
        output = self.kendall_layer(
            query_feat, support_feat, self.way_num, self.shot_num, self.query_num
        ).view(episode_size * self.way_num * self.query_num, self.way_num).to(self.device)
        acc = accuracy(output, query_target.reshape(-1))

        return output, acc

    def set_forward_loss(self, batch):
        """

        :param batch:
        :return:
        """
        image, global_target = batch
        image = image.to(self.device)
        episode_size = image.size(0) // (
            self.way_num * (self.shot_num + self.query_num)
        )
        feat = self.emb_func(image)
        support_feat, query_feat, support_target, query_target = self.split_by_episode(
            feat, mode=2
        )
        self.kendall_layer.change_mode(self.train_type)
        output = self.kendall_layer(
            query_feat,
            support_feat,
            self.way_num,
            self.shot_num,
            self.query_num,
        ).view(episode_size * self.way_num * self.query_num, self.way_num).to(self.device)
        loss = self.loss_func(output, query_target.reshape(-1))
        acc = accuracy(output, query_target.reshape(-1))
        return output, acc, loss