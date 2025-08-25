# 导入所需的包
import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
from torchvision import models
from torch.utils.data import DataLoader, random_split
from torchvision import datasets, transforms
from tqdm import tqdm

# 下载数据集，并且定义训练和测试图像变换操作
train_transform = transforms.Compose([
transforms.RandomResizedCrop(224),
# You can add more transform here...
# ...
transforms.ToTensor(),
# If you use ImageNet1K pretrained-model, normalize as follow
transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
])
test_transform = transforms.Compose([
transforms.Resize(256),
transforms.CenterCrop(224),
transforms.ToTensor(),
# If you use ImageNet1K pretrained-model, normalize as follow
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

class VGG(nn.Module):
    def __init__(self, features, num_classes=1000, init_weights=False):
        super(VGG, self).__init__()
        self.features = features			# 卷积层提取特征
        self.classifier = nn.Sequential(	# 全连接层进行分类
            nn.Dropout(p=0.5),
            nn.Linear(512*7*7, 2048),
            nn.ReLU(True),
            nn.Dropout(p=0.5),
            nn.Linear(2048, 2048),
            nn.ReLU(True),
            nn.Linear(2048, num_classes)
        )
        if init_weights:
            self._initialize_weights()
 
    def forward(self, x):
        # N x 3 x 224 x 224
        x = self.features(x)
        # N x 512 x 7 x 7
        x = torch.flatten(x, start_dim=1)
        # N x 512*7*7
        x = self.classifier(x)
        return x
 
    def _initialize_weights(self):
        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                # nn.init.kaiming_normal_(m.weight, mode='fan_out', nonlinearity='relu')
                nn.init.xavier_uniform_(m.weight)
                if m.bias is not None:
                    nn.init.constant_(m.bias, 0)
            elif isinstance(m, nn.Linear):
                nn.init.xavier_uniform_(m.weight)
                # nn.init.normal_(m.weight, 0, 0.01)
                nn.init.constant_(m.bias, 0)

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
        scheduler.step()
        epoch_loss = running_loss / len(train_loader.dataset)
        epoch_accuracy = 100 * correct / total
        print(f'Epoch [{epoch+1}/{num_epochs}], Loss: {epoch_loss:.4f}, Train Accuracy: {epoch_accuracy:.2f}%')

def evaluate_model(num_epochs, model, criterion, test_loader, device):
    model.eval()  # 将模型设置为评估模式
    with torch.no_grad():  # 在评估过程中不需要计算梯度
        total = 0
        correct = 0
        for epoch in range(num_epochs):
            print(f'evaluating on epoch:{epoch}')
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
# 实例化模型:此处可以选择是否预训练 (pytorch版本更新后pretrained名称已经弃用，改为了weights);注意：一共102分类项
model = VGG(models.vgg16(weights=None).features, num_classes=102, init_weights=True)
# 采用gpu
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model.to(device)
# Define loss function and optimizer
epochs = 10
criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(model.parameters(), lr=0.01, momentum=0.9, weight_decay=2e-4)
scheduler = optim.lr_scheduler.CosineAnnealingLR(optimizer, T_max=epochs)
# Run training and evaluation
train_model(epochs, model, criterion, optimizer, scheduler, train_loader, device)
evaluate_model(epochs,model, criterion, test_loader, device)
