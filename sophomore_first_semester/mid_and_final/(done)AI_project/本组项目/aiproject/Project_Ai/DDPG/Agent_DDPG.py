import random
import numpy as np
import gym
import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import matplotlib.pyplot as plt

# 定义Actor网络

class Actor(nn.Module):
    def __init__(self, state_dim, action_dim, hidden_size):
        super(Actor, self).__init__()
        self.fc1 = nn.Linear(state_dim, hidden_size)
        self.fc2 = nn.Linear(hidden_size, hidden_size)
        self.fc3 = nn.Linear(hidden_size, action_dim)

    def forward(self, x):
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = torch.tanh(self.fc3(x))
        return x

# 定义Critic网络


class Critic(nn.Module):
    def __init__(self, state_dim, action_dim, hidden_size):
        super(Critic, self).__init__()
        self.fc1 = nn.Linear(state_dim + action_dim, hidden_size)
        self.fc2 = nn.Linear(hidden_size, hidden_size)
        self.fc3 = nn.Linear(hidden_size, 1)

    def forward(self, state, action):
        x = torch.cat([state, action], 1)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

# 定义经验池


class ReplayBuffer:
    def __init__(self, capacity):
        self.capacity = capacity
        self.buffer = []
        self.position = 0

    def push(self, state, action, next_state, reward, done):
        if len(self.buffer) < self.capacity:
            self.buffer.append(None)
        self.buffer[self.position] = (state, action, next_state, reward, done)
        self.position = (self.position + 1) % self.capacity

    def sample(self, batch_size):
        batch = random.sample(self.buffer, batch_size)
        state, action, next_state, reward, done = map(np.stack, zip(*batch))
        return state, action, next_state, reward, done

    def __len__(self):
        return len(self.buffer)

# 定义DDPG智能体


class DDPGAgent:
    def __init__(self, state_dim, action_dim, hidden_size, actor_lr, critic_lr, gamma, tau, buffer_capacity):
        self.actor = Actor(state_dim, action_dim, hidden_size)
        self.actor_target = Actor(state_dim, action_dim, hidden_size)
        self.critic = Critic(state_dim, action_dim, hidden_size)
        self.critic_target = Critic(state_dim, action_dim, hidden_size)
        self.actor_optim = optim.Adam(self.actor.parameters(), lr=actor_lr)
        self.critic_optim = optim.Adam(self.critic.parameters(), lr=critic_lr)
        self.gamma = gamma
        self.tau = tau
        self.memory = ReplayBuffer(buffer_capacity)

    def get_action(self, state):
        state = torch.FloatTensor(state).unsqueeze(0)
        action = self.actor(state).squeeze(0).detach().numpy()
        return np.clip(action, -2, 2)

    def update(self, batch_size):
        if len(self.memory) < batch_size:
            return
        # 从经验池sample
        state, action, next_state, reward, done = self.memory.sample(
            batch_size)
        state = torch.FloatTensor(state)
        action = torch.FloatTensor(action)
        next_state = torch.FloatTensor(next_state)
        reward = torch.FloatTensor(reward)
        # 是否为结束状态
        done = torch.FloatTensor(done)

        # 更新Critic网络
        # self.actor_target(next_state)是下一个状态的动作值
        # self.critic_target(next_state, self.actor_target(next_state))是下一个状态的Q值
        # 它表示了从下一个状态出发，智能体所能获得的期望回报
        # 注意：这里使用了目标网络来计算下一个状态的Q值和动作值，来稳定训练过程。
        target_q = reward + (1 - done) * self.gamma * \
            self.critic_target(next_state, self.actor_target(next_state))
        q = self.critic(state, action)
        # critic_loss计算的是智能体预测的Q值与目标Q值之间的均方误差。
        # 这里假设target_q是最优策略，但实际上它是根据当前策略所能得到的最优Q值来计算的，而不是全局最优策略对应的Q值。
        critic_loss = F.mse_loss(q, target_q)
        self.critic_optim.zero_grad()
        critic_loss.backward()
        self.critic_optim.step()

        # 更新Actor网络
        # 奖励值越大，loss越小
        actor_loss = -self.critic(state, self.actor(state)).mean()
        self.actor_optim.zero_grad()
        actor_loss.backward()
        self.actor_optim.step()

        # 更新目标网络
        for target_param, param in zip(self.actor_target.parameters(), self.actor.parameters()):
            target_param.data.copy_(
                self.tau * param.data + (1 - self.tau) * target_param.data)
        for target_param, param in zip(self.critic_target.parameters(), self.critic.parameters()):
            target_param.data.copy_(
                self.tau * param.data + (1 - self.tau) * target_param.data)

    def save(self, filename):
        torch.save([self.actor.state_dict(),
                   self.critic.state_dict()], filename)

    def load(self, filename):
        actor_state_dict, critic_state_dict = torch.load(filename)
        self.actor.load_state_dict(actor_state_dict)
        self.actor_target.load_state_dict(actor_state_dict)
        self.critic.load_state_dict(critic_state_dict)
        self.critic_target.load_state_dict(critic_state_dict)

# 训练智能体


def train(agent, env, episodes, steps_per_episode, batch_size):
    rewards = []
    for i in range(episodes):
        state = env.reset()
        episode_reward = 0
        for j in range(steps_per_episode):
            # actor网络计算action
            action = agent.get_action(state)
            next_state, reward, done, _ = env.step(action)
            # 记录到经验池
            agent.memory.push(state, action, next_state, reward, done)
            state = next_state
            episode_reward += reward
            agent.update(batch_size)
        rewards.append(episode_reward)
        print('Episode %d, Reward: %.2f' % (i, episode_reward))
    return rewards

# 测试智能体


def test(agent, env, episodes, steps_per_episode):
    for i in range(episodes):
        state = env.reset()
        episode_reward = 0
        for j in range(steps_per_episode):
            action = agent.get_action(state)
            next_state, reward, done, _ = env.step(action)
            state = next_state
            episode_reward += reward
            env.render()
            if done:
                break
        print('Episode %d, Reward: %.2f' % (i, episode_reward))


# 训练和测试
env = gym.make('Pendulum-v1')
state_dim = env.observation_space.shape[0]
action_dim = env.action_space.shape[0]
hidden_size = 64
actor_lr = 1e-3
critic_lr = 1e-3
gamma = 0.99
tau = 0.001
buffer_capacity = int(1e6)
batch_size = 128
episodes = 100
steps_per_episode = 200

agent = DDPGAgent(state_dim, action_dim, hidden_size,
                  actor_lr, critic_lr, gamma, tau, buffer_capacity)

rewards = train(agent, env, episodes, steps_per_episode, batch_size)

test(agent, env, 1, steps_per_episode)

# 可视化训练过程
plt.plot(rewards)
plt.xlabel('Episode')
plt.ylabel('Reward')
plt.show()
