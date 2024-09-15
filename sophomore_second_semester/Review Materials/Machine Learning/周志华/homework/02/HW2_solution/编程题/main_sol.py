from utils import verify_sha256, load_coil20
from utils import LDAMeta, RidgeMeta
from utils import lda_sanity_check, classifier_2_sanity_check, classifier_n_sanity_check

import multiprocessing as mp

import tqdm
import numpy as np
import matplotlib.pyplot as plt

# NOTE 你可能会用到这两个函数
from scipy.linalg import pinv, eig

COIL20_ZIP_PATH = "./coil-20-proc.zip"
COIL20_HASH_STR = "517c5594820eb40066ba0ff6842e7f09392bf7fde849bf9cb9c28445b0f29e88"


class LDA(LDAMeta):
    """LDA 类, 提供 fit 和 transform 两个方法"""

    def __init__(self, n_dimension: int) -> None:
        """
        初始化超参数

        参数
            n_dimension: int
                需要降维到的维度
        """

        assert n_dimension > 0
        self.n_dimension = n_dimension

    def fit(self, x: np.ndarray, y: np.ndarray) -> None:
        """
        训练模型

        参数
            x: np.ndarray
                形状为 (train_size, dimension) 图像数据, 类型 np.float32
            y: np.ndarray
                形状为 (train_size,) 标签数据, 顺序对应于 x, 类型为 np.int32

        返回
            无返回值
        """

        # n 是类别个数; m 是训练样本数量; d 是样例维度.
        n = y.max() + 1
        m, d = x.shape

        # TODO

        mu = x.mean(axis=0)
        centered = x - mu
        St = centered.T @ centered

        Sw = np.zeros((d, d))
        group = {k: [] for k in range(n)}
        for i in range(m):
            group[y[i]].append(x[i])
        for k in range(n):
            group[k] = np.stack(group[k])
            mu_k = group[k].mean(axis=0)
            centered_k = group[k] - mu_k
            Sw_k = centered_k.T @ centered_k
            Sw += Sw_k

        Sb = St - Sw

        eigvals, eigvecs = eig(Sb, Sw)

        # decending, row vec
        eigvals = eigvals.real.astype(np.float32)
        eigvecs = eigvecs.real.astype(np.float32)
        eiginds = np.argsort(eigvals)[::-1][: self.n_dimension]
        eigvals = eigvals[eiginds]
        eigvecs = eigvecs.T[eiginds]

        self.eigvecs = eigvecs

    def transform(self, x: np.ndarray) -> np.ndarray:
        """
        测试模型

        参数
            x: np.ndarray
                形状为 (test_size, dimension) 图像数据, 类型 np.float32

        返回
            y: np.ndarray
                形状为 (test_size, 2) 标签数据, 顺序对应于 x, 类型为 np.float32
        """

        # n 是类别个数; m 是训练样本数量; d 是样例维度.
        m, d = x.shape

        # TODO

        y = x @ self.eigvecs.T
        return y.astype(np.float32)


class Ridge2(RidgeMeta):
    """Ridge2 类, 提供 fit 和 predict 两个方法"""

    def __init__(self, Lambda: float) -> None:
        """
        初始化超参数

        参数
            Lambda: float
                正则化系数 λ
        """

        assert Lambda > 0.0
        self.Lambda = Lambda

    def fit(self, x: np.ndarray, y: np.ndarray) -> None:
        """
        训练模型

        参数
            x: np.ndarray
                形状为 (train_size, dimension) 图像数据, 类型 np.float32
            y: np.ndarray
                形状为 (train_size,) 标签数据, 顺序对应于 x, 类型为 np.int32

        返回
            无返回值
        """

        # n 是类别个数; m 是训练样本数量; d 是样例维度.
        n = y.max() + 1
        m, d = x.shape

        # 这里约定只有两类
        assert n == 2

        # TODO

        yr = np.where(y == 0, -1, 1)
        x1 = np.hstack((x, np.ones((m, 1))))
        w = pinv(x1.T @ x1 + self.Lambda * np.identity(d + 1)) @ x1.T @ yr.reshape((-1, 1))
        self.w = w.reshape((-1,))

    def predict(self, x: np.ndarray) -> np.ndarray:
        """
        测试模型

        参数
            x: np.ndarray
                形状为 (test_size, dimension) 图像数据, 类型 np.float32

        返回
            y: np.ndarray
                形状为 (test_size,) 标签数据, 顺序对应于 x, 类型为 np.int32
        """

        # n 是类别个数; m 是训练样本数量; d 是样例维度.
        m, d = x.shape

        # TODO

        x1 = np.hstack((x, np.ones((m, 1))))
        yr = x1 @ self.w.reshape((-1, 1))
        y = np.where(yr <= 0, 0, 1)
        return y.reshape((-1,)).astype(np.int32)


class RidgeN(RidgeMeta):
    """RidgeN 类, 提供 fit 和 predict 两个方法"""

    def __init__(self, Lambda: float) -> None:
        """
        初始化超参数

        参数
            Lambda: float
                正则化系数 λ
        """

        assert Lambda > 0.0
        self.Lambda = Lambda

    def fit(self, x: np.ndarray, y: np.ndarray) -> None:
        """
        训练模型

        参数
            x: np.ndarray
                形状为 (train_size, dimension) 图像数据, 类型 np.float32
            y: np.ndarray
                形状为 (train_size,) 标签数据, 顺序对应于 x, 类型为 np.int32

        返回
            无返回值
        """

        # n 是类别个数; m 是训练样本数量; d 是样例维度.
        n = y.max() + 1
        m, d = x.shape

        self.n = n
        self.ridge2_pool = [Ridge2(self.Lambda) for k in range(n)]

        # TODO

        for k in range(n):
            self.ridge2_pool[k].fit(x, np.where(y == k, 1, 0))

    def predict(self, x: np.ndarray) -> np.ndarray:
        """
        测试模型

        参数
            x: np.ndarray
                形状为 (test_size, dimension) 图像数据, 类型 np.float32

        返回
            y: np.ndarray
                形状为 (test_size,) 标签数据, 顺序对应于 x, 类型为 np.int32
        """

        # n 是类别个数; m 是训练样本数量; d 是样例维度.
        m, d = x.shape

        # TODO

        result_pool = [None for k in range(self.n)]
        for k in range(self.n):
            result_pool[k] = self.ridge2_pool[k].predict(x)
        result_pool = np.stack(result_pool)
        y = result_pool.argmax(axis=0)
        return y.astype(np.int32)


def main():
    verify_sha256(COIL20_ZIP_PATH, COIL20_HASH_STR)
    x_train, y_train, x_test, y_test = load_coil20(COIL20_ZIP_PATH)

    # 检查输入输出格式

    lda_sanity_check(LDA)
    classifier_2_sanity_check(Ridge2)
    classifier_n_sanity_check(RidgeN)

    # 把训练数据降至两维并绘制散点图

    lda = LDA(n_dimension=2)
    lda.fit(x_train, y_train)
    x_test_2d = lda.transform(x_test)

    # TODO

    n = 20
    m, d = x_test_2d.shape
    colors = plt.cm.get_cmap("tab20", 20)
    plt.figure(figsize=(8, 8), dpi=256)
    group = {k: [] for k in range(n)}
    for i in range(m):
        group[y_test[i]].append(x_test_2d[i])
    for k in range(n):
        group[k] = np.stack(group[k])
        plt.scatter(group[k][:, 0], group[k][:, 1], c=colors(k), label=str(k))
    plt.legend()
    plt.xlabel("LDA dim. 1")
    plt.ylabel("LDA dim. 2")
    plt.title("LDA dim. reduction on test set")
    plt.legend()
    plt.savefig("scatter_test.jpg")

    # 把训练数据降至两维并绘制散点图

    lda = LDA(n_dimension=2)
    lda.fit(x_train, y_train)
    x_train_2d = lda.transform(x_train)

    # TODO

    n = 20
    m, d = x_train_2d.shape
    colors = plt.cm.get_cmap("tab20", 20)
    plt.figure(figsize=(8, 8), dpi=256)
    group = {k: [] for k in range(n)}
    for i in range(m):
        group[y_train[i]].append(x_train_2d[i])
    for k in range(n):
        group[k] = np.stack(group[k])
        plt.scatter(group[k][:, 0], group[k][:, 1], c=colors(k), label=str(k))
    plt.legend()
    plt.xlabel("LDA dim. 1")
    plt.ylabel("LDA dim. 2")
    plt.title("LDA dim. reduction on train set")
    plt.legend()
    plt.savefig("scatter_train.jpg")

    # 训练错误率和测试错误率随 λ 变化的折线图

    Lambda_seq = np.logspace(start=-4, stop=4, num=9).tolist()

    # TODO

    train_error = []
    test_error = []
    for Lambda in tqdm.tqdm(Lambda_seq):
        ridge_n = RidgeN(Lambda)
        ridge_n.fit(x_train, y_train)
        y_train_pred = ridge_n.predict(x_train)
        train_error.append(1 - np.mean(y_train_pred == y_train))
        y_test_pred = ridge_n.predict(x_test)
        test_error.append(1 - np.mean(y_test_pred == y_test))

    # # 大致如此:
    # Lambda_seq = [0.0001, 0.001, 0.01, 0.1, 1.0, 10.0, 100.0, 1000.0, 10000.0]
    # train_error = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
    # test_error = [0.56590909, 0.56590909, 0.52954545, 0.44545454, 0.34318181, 0.23181818, 0.14090909, 0.08409090, 0.06818181]

    plt.figure(figsize=(8, 8), dpi=256)
    plt.plot(Lambda_seq, train_error, label="train error")
    plt.plot(Lambda_seq, test_error, label="test error")
    plt.xlabel("$\\lambda$")
    plt.ylabel("error")
    plt.xscale('log')
    plt.title("train/test error w.r.t. $\\lambda$")
    plt.legend()
    plt.savefig("line.jpg")


if __name__ == "__main__":
    main()
