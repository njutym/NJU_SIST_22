import numpy as np
from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score

# 加载数据集
iris = load_iris()
X = iris.data
y = iris.target

# 设置迭代次数
num_iterations = 10
accuracies = []

for i in range(num_iterations):
    # 划分训练集和测试集
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=i)

    # 创建并拟合决策树模型
    clf = DecisionTreeClassifier(criterion='gini', splitter='best', max_depth=3)
    clf.fit(X_train, y_train)

    # 进行预测并计算准确率
    y_pred = clf.predict(X_test)
    score = accuracy_score(y_test, y_pred)
    accuracies.append(score)

# 计算平均准确率
mean_accuracy = np.mean(accuracies)
print("平均准确率: ", mean_accuracy)