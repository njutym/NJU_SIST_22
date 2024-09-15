# 导入所需的包
import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torchvision import models
from torch.utils.data import DataLoader, random_split
from torchvision import datasets, transforms
from tqdm import tqdm

import os
import numpy as np
import matplotlib.pyplot as plt
from sklearn.manifold import TSNE

CUDA_VISIBLE_DEVICES = 1


# 下载数据集，并且定义训练和测试图像变换操作
train_transform = transforms.Compose([
transforms.RandomResizedCrop(224),
transforms.RandomHorizontalFlip(),  # 添加随机水平翻转
transforms.ColorJitter(brightness=0.2, contrast=0.2, saturation=0.2, hue=0.1),  # 随机颜色调整
transforms.RandomRotation(30),  # 添加随机旋转
transforms.ToTensor(),
# If you use ImageNet1K pretrained-model, normalize as follow
transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
])
test_transform = transforms.Compose([
transforms.Resize(256),
transforms.CenterCrop(224),
transforms.ToTensor(),
transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
])


# 定义数据集和数据加载器
dataset_path = 'D:/@extremely_important_files/class_and_homework/sophomore_second_semester/Machine Learning/homework/02/dataset/caltech-101/caltech-101/101_ObjectCategories' 
dataset = datasets.ImageFolder(root=dataset_path, transform=train_transform)
# Split dataset into training and testing
train_size = int(0.8 * len(dataset))
test_size = len(dataset) - train_size
train_dataset, test_dataset = random_split(dataset, [train_size, test_size])
# Apply different transforms to test dataset
test_dataset.dataset.transform = test_transform
# Create data loaders
train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)


# 定义模型
model = models.vgg16(pretrained='Default') # 加载torch原本的vgg16模型，设置pretrained=True，即使用预训练模型
num_fc = model.classifier[6].in_features # 获取最后一层的输入维度
model.classifier[6] = torch.nn.Linear(num_fc, 102)# 修改最后一层的输出维度，即分类数
# 对于模型的每个权重，使其不进行反向传播，即固定参数
for param in model.parameters():
    param.requires_grad = False
# 将分类器的最后层输出维度换成了num_cls，这一层需要重新学习
for param in model.classifier[6].parameters():
    param.requires_grad = True



# 定义全局变量用于记录训练过程中的精度和损失
train_losses = []
train_accuracies = []


# 定义全局变量用于记录每个epoch的特征向量和对应的标签
all_features = []
all_labels = []


# 定义训练与测试流程
def train_model(num_epochs, model, criterion, optimizer, scheduler,train_loader, device):
    model.train()
    for epoch in range(num_epochs):
        print(f'training on epoch:{epoch}')
        running_loss = 0.0
        total = 0
        correct = 0
        for images, labels in tqdm(train_loader):
            images, labels = images.to(device), labels.to(device)
            optimizer.zero_grad()
            outputs = model(images)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()
            running_loss += loss.item() * images.size(0)
            _, predicted = torch.max(outputs, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()

            # 保存特征向量和标签
            all_features.append(model.features(images).detach().cpu().numpy())
            all_labels.append(labels.detach().cpu().numpy())
        
        scheduler.step()
        epoch_loss = running_loss / len(train_loader.dataset)
        epoch_accuracy = 100 * correct / total

        # 保存数据
        train_losses.append(epoch_loss)
        train_accuracies.append(epoch_accuracy)
        
        print(f'Epoch [{epoch+1}/{num_epochs}], Loss: {epoch_loss:.4f}, Train Accuracy: {epoch_accuracy:.2f}%')

def evaluate_model(model, criterion, test_loader, device):
    model.eval()  # 将模型设置为评估模式
    total = 0
    correct = 0
    with torch.no_grad():  # 在评估过程中不需要计算梯度
        for images, labels in tqdm(test_loader):
            images, labels = images.to(device), labels.to(device)
            outputs = model(images)
            _, predicted = torch.max(outputs, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()

    accuracy = 100 * correct / total
    print(f'Test Accuracy: {accuracy:.2f}%')

# 主函数
# Create data loaders
train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=32, shuffle=False)

# 采用gpu
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# 将模型和数据移到 GPU 上
model.to(device)  # 将模型移到 GPU 上

# Define loss function and optimizer
epochs = 10
criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.9, weight_decay=2e-4)
scheduler = optim.lr_scheduler.CosineAnnealingLR(optimizer, T_max=epochs)
# Run training and evaluation
train_model(epochs, model, criterion, optimizer, scheduler, train_loader, device)
evaluate_model(model, criterion, test_loader, device)

# 绘制训练集精度和损失曲线
plt.figure(figsize=(10, 5))
plt.subplot(1, 2, 1)
plt.plot(train_accuracies, label='Train Accuracy')
plt.xlabel('Epoch')
plt.ylabel('Accuracy (%)')
plt.title('Training Accuracy')

plt.subplot(1, 2, 2)
plt.plot(train_losses, label='Train Loss')
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.title('Training Loss')

plt.show()
