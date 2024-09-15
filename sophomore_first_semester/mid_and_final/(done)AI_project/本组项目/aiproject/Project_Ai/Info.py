import pymem
import os
import time
from torch import tensor
from torch import stack
from torch import cat

import copy
import time
import random
import torch
from torch import nn
from torch import optim
import gym
import numpy as np
import matplotlib.pyplot as plt
from collections import deque, namedtuple       # 队列类型
from tqdm import tqdm                           # 绘制进度条用
import Env

exe = pymem.Pymem("th123_beta.exe")

action = {
    0: 'DoNothing',
    1: 'ToDown',
    2: 'Down',
    3: 'Up',
    4: 'Walk_Forward',
    5: 'Walk_Backward',
    6: 'Jump',
    7: 'Jump_Forward',
    8: 'Jump_Backward',
    9: 'Drop',
    10: 'Land',
    50: 'Hitten_Up',
    51: 'Hitten',
    52: 'Hitten',
    57: 'Hitten_Mid',
    72: 'BANDAO',
    74: 'Hitten_Fly',
    76: 'knkback',
    78: 'Wall',
    97: 'Hitten_Down',
    98: 'ToUp',
    199: 'Revive',
    200: 'Dash_Forward',
    201: 'Dash_Backward',
    204: 'Dash_End',
    214: 'Fly',
    301: 'Hit',
    306: 'Hit_Air',
    307: 'Hit_Air2',
    400: 'Shoot',
    402: 'Shoot_Down',
    404: 'Shoot_Air',
    410: 'Shoot2',
    414: 'Shoot2_Air'
}


class Player(object):

    def __init__(self, id) -> None:
        self.id = id
        self.id_addr = 0
        self.hp_addr = 0
        self.base = 0
        self.state = {}
        self.pre_state = {}

    def init(self):
        if self.id == 0:
            self.id_addr = 0x886CF0
            self.hp_addr = exe.read_int(0x8855c8)+0x4A4
            self.base = exe.read_int(0x88562c)
        else:
            self.id_addr = 0x886D10
            self.hp_addr = exe.read_int(0x8855c8)+0x59C
            self.base = exe.read_int(0x885630)

    def sync(self):
        # exe = pymem.Pymem("th123_beta.exe")
        self.state['id'] = exe.read_int(self.id_addr)
        self.state['hp'] = exe.read_int(self.hp_addr)
        self.state['x'] = exe.read_int(self.base)+0x220
        self.state['mp'] = exe.read_int(self.base+0x3AC)
        self.state['xp'] = exe.read_int(self.base+0x4F4)-83886080
        self.state['y'] = exe.read_int(self.base+0xE4)
        self.state['behavior'] = exe.read_short(self.base+0xD8)
        print(self.id,end=' ')
        print(self.state['hp'])


P_0 = Player(0)
P_1 = Player(1)


def init():
    P_0.init()
    P_1.init()


def renew():
    P_0.sync()
    P_1.sync()

def show():
    os.system('cls')
    P_0.sync()
    print("Player_{}: {}".format(
        P_0.id, P_0.state))
    print('Player_0: {}'.format(
        action[P_0.state['behavior']]if P_0.state['behavior'] in action.keys() else 'Unknown'))
    P_1.sync()
    print("Player_{}: {}".format(
        P_1.id, P_1.state))
    print('Player_1: {}'.format(
        action[P_1.state['behavior']]if P_1.state['behavior'] in action.keys() else 'Unknown'))


def obv():
    # return cat([tensor(list(P_0.state.values())), tensor(list(P_1.state.values()))])
    return list(P_0.state.values())+list(P_1.state.values())


def eval():
    reward = (P_0.state['hp']-P_0.pre_state['hp']) - (P_1.state['hp']-P_1.pre_state['hp']) 
    reward /= 100
    if P_0.state['x'] == 20 or P_0.state['x'] == 1220:
        reward = -0.5
    #!!!!!print(P_0.state)
    P_0.pre_state = copy.deepcopy(P_0.state)
    P_1.pre_state = copy.deepcopy(P_1.state)
    # P_0.pre_state = P_0.state#！！！！！！！！！！浅拷贝
    # P_1.pre_state = P_1.state#
    return reward


def done():#!!!!!!!!!!!!!!
    return P_0.state['hp'] == 0 or P_1.state['hp'] == 0
    return P_0.state['hp'] - P_1.state['hp'] <= 5000 or P_1.state['hp'] - P_0.state['hp'] <= 5000


init()
renew()
P_0.pre_state = copy.deepcopy(P_0.state)
P_1.pre_state = copy.deepcopy(P_1.state)
# P_0.pre_state = P_0.state#!!!!!.copy()
# P_1.pre_state = P_1.state
print(obv())
# while True:
#     eval()
#     time.sleep(0.1)
#     renew()
#     # print(P_1.state['hp'], P_1.pre_state['hp'])
#     print(eval())



# from Env import env
device = torch. device("cuda" if torch.cuda.is_available() else "cpu")
Transition = namedtuple(
    'Transition', ('state', 'action', 'reward', 'next_state', 'done'))
# Transition = namedtuple(
#     'Transition', ('state', 'action', 'reward', 'next_state', 'done'))


class ReplayMemory(object):

    def __init__(self, memory_size):
        self.memory = deque([], maxlen=memory_size)

    def sample(self, batch_size):
        batch_data = random.sample(self.memory, batch_size)
        state, action, reward, next_state, done = zip(*batch_data)
        return state, action, reward, next_state, done

    def push(self, *args):
        # *args: 把传进来的所有参数都打包起来生成元组形式
        # self.push(1, 2, 3, 4, 5)
        # args = (1, 2, 3, 4, 5)
        self.memory.append(Transition(*args))

    def __len__(self):
        return len(self.memory)


class Qnet(nn.Module):

    def __init__(self, n_observations, n_actions):
        super(Qnet, self).__init__()
        self.model = nn.Sequential(
            nn.Linear(n_observations, 128),
            nn.ReLU(),
            nn.Linear(128, n_actions)
        )

    def forward(self, state):
        return self.model(state)


class Agent(object):

    def __init__(self, observation_dim, action_dim, gamma, lr, epsilon, target_update):
        self.action_dim = action_dim
        self.q_net = Qnet(observation_dim, action_dim).to(device)
        self.target_q_net = Qnet(observation_dim, action_dim).to(device)
        self.gamma = gamma
        self.lr = lr
        self.epsilon = epsilon
        self.target_update = target_update
        self.count = 0

        self.optimizer = optim.Adam(params=self.q_net.parameters(), lr=lr)
        self.loss = nn.MSELoss()

    def take_action(self, state):
        if np.random.uniform(0, 1) < 1 - self.epsilon:
            state = torch.tensor(state, dtype=torch.float).to(device)
            action = torch.argmax(self.q_net(state)).item()
        else:
            action = np.random.choice(self.action_dim)
        return action

    def update(self, transition_dict):

        states = transition_dict.state
        actions = np.expand_dims(transition_dict.action, axis=-1)  # 扩充维度
        rewards = np.expand_dims(transition_dict.reward, axis=-1)  # 扩充维度
        next_states = transition_dict.next_state
        dones = np.expand_dims(transition_dict.done, axis=-1)  # 扩充维度

        states = torch.tensor(states, dtype=torch.float).to(device)
        actions = torch.tensor(actions, dtype=torch.int64).to(device)
        rewards = torch.tensor(rewards, dtype=torch.float).to(device)
        next_states = torch.tensor(next_states, dtype=torch.float).to(device)
        dones = torch.tensor(dones, dtype=torch.float).to(device)

        # update q_values
        # gather(1, acitons)意思是dim=1按行号索引， index=actions
        # actions=[[1, 2], [0, 1]] 意思是索引出[[第一行第2个元素， 第1行第3个元素],[第2行第1个元素， 第2行第2个元素]]
        # 相反，如果是这样
        # gather(0, acitons)意思是dim=0按列号索引， index=actions
        # actions=[[1, 2], [0, 1]] 意思是索引出[[第一列第2个元素， 第2列第3个元素],[第1列第1个元素， 第2列第2个元素]]
        # states.shape(64, 4) actions.shape(64, 1), 每一行是一个样本，所以这里用dim=1很合适
        predict_q_values = self.q_net(states).gather(1, actions)
        with torch.no_grad():
            # max(1) 即 max(dim=1)在行向找最大值，这样的话shape(64, ), 所以再加一个view(-1, 1)扩增至(64, 1)
            max_next_q_values = self.target_q_net(
                next_states).max(1)[0].view(-1, 1)
            q_targets = rewards + self.gamma * max_next_q_values * (1 - dones)
        l = self.loss(predict_q_values, q_targets)

        self.optimizer.zero_grad()
        l.backward()
        self.optimizer.step()
        if self.count % self.target_update == 0:
            # copy model parameters
            self.target_q_net.load_state_dict(self.q_net.state_dict())

        self.count += 1


def run_episode(env, agent, repalymemory, batch_size):
    state = env.reset()
    reward_total = 0
    step = 0
    while True:
        renew()#!!!!!!!!!!!
        step += 1
        action = agent.take_action(state)
        next_state, reward, done, _ = env.step(action)
        print(reward)
        print(state)
        print(P_0.state)
        repalymemory.push(state, action, reward, next_state, done)
        if len(repalymemory) > batch_size and (step % 5 == 0):#!!!!!!!!!!
            state_batch, action_batch, reward_batch, next_state_batch, done_batch = repalymemory.sample(
                batch_size)
            T_data = Transition(state_batch, action_batch,
                                reward_batch, next_state_batch, done_batch)
            # print(T_data)
            agent.update(T_data)
        reward_total += reward
        state = next_state #!!!!!!!
        if done:
            break
    return reward_total


def episode_evaluate(env, agent, render):
    reward_list = []
    for i in range(5):
        state = env.reset()
        reward_episode = 0
        while True:
            action = agent.take_action(state)
            next_state, reward, done, _ = env.step(action)
            reward_episode += reward
            state = copy.deepcopy(next_state)#!!!!!!!!!!!
            if done:
                break
        reward_list.append(reward_episode)
    return np.mean(reward_list).item()

time.sleep(3)
print('start')
#
if __name__ == "__main__":

    env = Env.Env()
    observation_n, action_n = env.observation_space, env.action_space
    # print(observation_n, action_n)
    agent = Agent(observation_n, action_n, gamma=0.98,
                  lr=2e-3, epsilon=0.01, target_update=10)

    replaymemory = ReplayMemory(memory_size=10000)
    batch_size = 64

    num_episodes = 200
    reward_list = []
    # print("start to train model")
    # 显示10个进度条
    for i in range(10):
        with tqdm(total=int(num_episodes/10), desc="Iteration %d" % i) as pbar:
            for episode in range(int(num_episodes / 10)):
                reward_episode = run_episode(
                    env, agent, replaymemory, batch_size)
                reward_list.append(reward_episode)
                if (episode+1) % 10 == 0:

                    test_reward = episode_evaluate(env, agent, False)
                    # print("Episode %d, total reward: %.3f" % (episode, test_reward))
                    pbar.set_postfix({
                        'episode': '%d' % (num_episodes / 10 * i + episode + 1),
                        'return': '%.3f' % (test_reward)
                    })
                pbar.update(1)
    episodes_list = list(range(len(reward_list)))
    plt.plot(episodes_list, reward_list)
    plt.xlabel('Episodes')
    plt.ylabel('Returns')
    plt.title('Double DQN on {}'.format('th123'))
    plt.show()


# save_path = './dqn_model.ckpt'
# agent.save(save_path)