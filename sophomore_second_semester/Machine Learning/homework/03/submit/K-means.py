from sklearn.datasets import load_iris
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import adjusted_rand_score, adjusted_mutual_info_score, fowlkes_mallows_score
from sklearn.model_selection import train_test_split

# 加载数据集
iris = load_iris()
data = pd.DataFrame(data=iris.data, columns=iris.feature_names)
target = iris.target
data['target'] = target

# 自定义K—means算法使用现成的K—means库
class KMeans:
    def __init__(self, n_clusters, max_iter=300):
        self.n_clusters = n_clusters
        self.max_iter = max_iter
    
    def fit(self, X):
        n_samples, n_features = X.shape
        
        # 随机初始化质心
        self.centroids = X[np.random.choice(n_samples, self.n_clusters, replace=False)]
        
        # 迭代优化质心
        for _ in range(self.max_iter):
            # 分配样本到最近的质心
            labels = self._assign_clusters(X)
            
            # 更新质心为每个簇的平均值
            new_centroids = np.array([X[labels == i].mean(axis=0) for i in range(self.n_clusters)])
            
            # 如果质心不再变化，则停止迭代
            if np.allclose(new_centroids, self.centroids):
                break
            
            self.centroids = new_centroids
        
        return labels,self.centroids
    
    def _assign_clusters(self, X):
        # 计算每个样本到每个质心的距离
        distances = np.linalg.norm(X[:, np.newaxis] - self.centroids, axis=2)
        
        # 找到距离最近的质心的索引
        return np.argmin(distances, axis=1)
    
# 计算总内聚度
def total_intra_cluster_distance(X, centroids, labels):
    total_distance = 0
    for i in range(len(centroids)):
        cluster_points = X[labels == i]
        centroid = centroids[i]
        total_distance += np.sum(np.linalg.norm(cluster_points - centroid, axis=1)) # 计算欧氏距离(2-norms)
    return total_distance

# 划分数据集为训练集和测试集（150=120+30）
X_train, X_test, y_train, y_test = train_test_split(data.values[:, :-1], data['target'], test_size=0.2, random_state=42)


# 使用K-means算法进行聚类

#---------------------------------------------
# 训练集
kmeans = KMeans(n_clusters=3)
train_labels,centroids = kmeans.fit(X_train)

# 计算总内聚度
total_distance = total_intra_cluster_distance(X_train, kmeans.centroids, train_labels)/120
print("Total intra-cluster distance (training set):", total_distance)

# 可视化聚类结果
plt.scatter(X_train[:, 0], X_train[:, 1], c=train_labels, cmap='viridis')
plt.scatter(kmeans.centroids[:, 0], kmeans.centroids[:, 1], marker='x', c='red', label='Centroids')
plt.title('K-means Clustering training set')
plt.xlabel('Feature 1')
plt.ylabel('Feature 2')
plt.legend()
plt.show()

#-----------------------------------------------
# 测试集，根据样本点到训练集已经训练好的中心（存储在centroids变量中）的距离来直接给标签
def test_assign_clusters(X):
    distances = np.linalg.norm(X[:, np.newaxis] - centroids, axis=2)     
    # 找到距离最近的质心的索引
    return np.argmin(distances, axis=1)

test_labels = test_assign_clusters(X_test)

# 计算测试机总内聚度
total_distance_test = total_intra_cluster_distance(X_test, kmeans.centroids, test_labels)/30
print("Total intra-cluster distance (testing set):", total_distance_test)

# 可视化聚类结果
plt.scatter(X_test[:, 0], X_test[:, 1], c=test_labels, cmap='viridis')
plt.scatter(kmeans.centroids[:, 0], centroids[:, 1], marker='x', c='red', label='training_Centroids')
plt.title('K-means Clustering testing set')
plt.xlabel('Feature 1')
plt.ylabel('Feature 2')
plt.legend()
plt.show()

# 兰德指数（Rand Index）：度量两个分配的相似程度，即它们在一致性和不一致性方面的比率。
# 互信息（Mutual Information）：度量聚类结果和真实标签之间的信息交集。
# Fowlkes-Mallows指数：度量两个聚类之间的相似性，是准确率和召回率的几何平均值。
# 这些指标越接近1，表示聚类结果越好。


# Evaluating performance metrics on the testing set
rand_index_test = adjusted_rand_score(y_test, test_labels)
mutual_info_test = adjusted_mutual_info_score(y_test, test_labels)
fowlkes_mallows_test = fowlkes_mallows_score(y_test, test_labels)

print("Adjusted Rand Index (testing set):", rand_index_test)
print("Adjusted Mutual Information (testing set):", mutual_info_test)
print("Fowlkes-Mallows Index (testing set):", fowlkes_mallows_test)


# 测试其他的数据集，只需要替换数据集即可

# 灵活搜索合适的K,要跑的话要适当修改
# best_k = 0
# best_score = 0
# for k in range (1 , 100): # 尝试不同的K值
# kmeans = KMeans(n_clusters=k )
# kmeans.fit(X)
# ch_score = calinski_harabaz_score (X, labels)
# if ch_score > best_score :
#   best_k = k
#   best_score = ch_score
# print ( f "Best␣K: {best_k } ,Best Calinski −Harabasz␣ Score : { best_score }"" )