from sklearn.datasets import fetch_openml
from sklearn.model_selection import train_test_split
from sklearn import tree
import matplotlib.pyplot as plt
from sklearn.metrics import accuracy_score
# iris=load_iris()
# x=np.array(iris.data)
# y=np.array(iris.target)
mnist = fetch_openml('mnist_784', version=1, parser='auto')
x_train ,x_test,y_train ,y_test=train_test_split(mnist.data,mnist.target,
test_size=0.2)
clf=tree.DecisionTreeClassifier(criterion='entropy',splitter='best',
max_depth=8)
clf=clf.fit(x_train ,y_train)
y_pred = clf.predict(x_test)
score = accuracy_score(y_test,y_pred)
print("准 确 率:",score)
