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
    x_train_2d = lda.transform(x_train)

    # TODO

    # 把测试数据降至两维并绘制散点图

    lda = LDA(n_dimension=2)
    lda.fit(x_train, y_train)
    x_test_2d = lda.transform(x_test)

    # TODO

    # 训练错误率和测试错误率随 λ 变化的折线图

    Lambda_seq = np.logspace(start=-4, stop=4, num=9).tolist()

    # TODO


if __name__ == "__main__":
    main()
