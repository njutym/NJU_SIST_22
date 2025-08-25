import io, os, sys, time, platform, hashlib, datetime, traceback, zipfile
from PIL import Image
import numpy as np
from sklearn.model_selection import StratifiedShuffleSplit


if platform.system() == "Windows":
    os.system("color")


def colorize_text(text: str, color: str) -> str:
    """
    利用ANSI转义符把文本着色为彩色文本

    参数
        text: str
            要着色的文本
        color: str
            要使用的颜色, 支持的颜色名如下: black, red, green, yellow, blue, magenta, cyan, white.

    返回
        colorized_text: str
            着色后的文本字符串
    """

    color_mapping = {"black": 30, "red": 31, "green": 32, "yellow": 33, "blue": 34, "magenta": 35, "cyan": 36, "white": 37}
    assert color.lower() in color_mapping, f"颜色不存在! color={repr(color)}"
    color_code = color_mapping[color.lower()] + 60
    return f"\033[1;{color_code}m{text}\033[0m"


def log_template(message: str, color: str) -> None:
    """
    在终端打印着色文本

    参数
        message: str
            要着色的文本
        color: str
            要使用的颜色, 详见函数 colorize_text

    返回
        无返回值
    """

    if not isinstance(message, str):
        message = str(message)
    dt_str = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
    print(f"[{dt_str}]", colorize_text(message, color), file=sys.stdout, flush=True)


def log_debug(message: str) -> None:
    """在终端打印黄色文本用于调试, 详见函数 log_template"""
    log_template(message, "yellow")


def log_process(message: str) -> None:
    """在终端打印蓝色文本以示开始, 详见函数 log_template"""
    log_template(message, "blue")


def log_success(message: str) -> None:
    """在终端打印绿色文本以成功结束, 详见函数 log_template"""
    log_template(message, "green")


def log_failure(message: str) -> None:
    """在终端打印红色文本以失败结束, 详见函数 log_template"""
    log_template(message, "red")


def safe_run(comment: str):
    """
    用于计时和捕获异常的装饰器工厂

    参数
        comment: str
            任务名称

    返回
        装饰器
    """

    def decorator(func):
        """
        用于计时和捕获异常的装饰器

        参数
            func:
                待被装饰的函数

        返回
            被装饰的函数
        """

        def wrapper(*args, **kwargs):
            """装饰后的函数"""
            start_time = time.time()
            log_process(f"{comment}开始...")
            try:
                result = func(*args, **kwargs)
                ascription = None
            except:
                result = None
                ascription = traceback.format_exc()
            end_time = time.time()
            elapsed_time = end_time - start_time
            if isinstance(ascription, str):
                log_failure(f"{comment}失败! (耗时{elapsed_time:.4f}秒钟)\n{ascription}")
                raise SystemExit()
            else:
                log_success(f"{comment}成功! (耗时{elapsed_time:.4f}秒钟)")
            return result

        return wrapper

    return decorator


@safe_run("校验数据集")
def verify_sha256(path: str, sha256_str: str) -> None:
    """
    读取路径为 path 的文件, 校验其 SHA256 之值是否等于 sha256_str.

    参数
        path: str
            文件路径
        sha256_str: str
            SHA256 之值, 小写十六进制

    返回
        无返回值
    """

    assert os.path.exists(path) and os.path.isfile(path), f"文件不存在! path={repr(path)}"
    sha256 = hashlib.sha256()
    with open(path, mode="rb") as file:
        while data := file.read(64 * 1024):
            sha256.update(data)
    assert (actual_str := sha256.hexdigest()) == sha256_str, f"SHA256校验码错误! actual_str={repr(actual_str)}"


@safe_run("加载数据集")
def load_coil20(path: str) -> tuple[np.ndarray, np.ndarray]:
    """
    加载经过预处理的 COIL-20 数据集的 ZIP 压缩包的路径 (coil-20-proc.zip)

    详见 https://www.cs.columbia.edu/CAVE/software/softlib/coil-20.php

    参数
        path: str
            文件路径

    返回 x_train, y_train, x_test, y_test
        x_train, x_test: np.ndarray
            形状为 (sample_size, dimension=1024) 图像数据, 类型 np.float32
        y_train, y_test: np.ndarray
            形状为 (sample_size,) 标签数据, 顺序对应于 x_train, x_test, 类型 np.int32
    """

    assert os.path.exists(path) and os.path.isfile(path), f"文件不存在! path={repr(path)}"
    label_list = []
    matrix_list = []
    with zipfile.ZipFile(path, mode="r") as zip_handler:
        for file_name in zip_handler.namelist():
            if file_name.lower().endswith(".png"):
                # 根据文件名抽取标签
                class_str = os.path.basename(file_name)[:-4].split("__")[0][3:]
                class_index = int(class_str) - 1
                label_list.append(class_index)
                # 解压文件名加载灰度图片
                with zip_handler.open(file_name) as file:
                    image_data = io.BytesIO(file.read())
                image_obj = Image.open(image_data).resize((32, 32))
                image_array = np.asarray(image_obj, dtype=np.float32)
                matrix_list.append(image_array)
    x = np.stack(matrix_list)
    y = np.array(label_list, dtype=np.int32)
    # 划分训练集和测试集
    sss = StratifiedShuffleSplit(n_splits=1, test_size=0.3055, random_state=sum(map(ord, "ML2024Spring")))
    ((train_index, test_index),) = sss.split(x, y)
    x_train = x[train_index].reshape((train_index.shape[0], -1))
    y_train = y[train_index].reshape((train_index.shape[0],))
    x_test = x[test_index].reshape((test_index.shape[0], -1))
    y_test = y[test_index].reshape((test_index.shape[0],))
    return x_train, y_train, x_test, y_test


class LDAMeta:
    """LDA 模板类, 提供 fit 和 transform 两个方法"""

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

        raise NotImplementedError()

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

        raise NotImplementedError()


class RidgeMeta:
    """Ridge2 / RidgeN 模板类, 提供 fit 和 predict 两个方法"""

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
            Lambda: float
                正则化系数 λ

        返回
            无返回值
        """

        raise NotImplementedError()

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

        raise NotImplementedError()


@safe_run("检查LDA的输入输出格式")
def lda_sanity_check(lda_class: type[LDAMeta]):
    rng = np.random.default_rng()
    (n,) = rng.integers(8, 16, size=1, dtype=np.int32)
    (m,) = rng.integers(512, 1024, size=1, dtype=np.int32)
    (d,) = rng.integers(512, 1024, size=1, dtype=np.int32)
    x_train = rng.random(size=(m, d), dtype=np.float32)
    y_train = rng.integers(0, n, size=(m,), dtype=np.int32)
    x_test = rng.random(size=(m, d), dtype=np.float32)
    lda = lda_class(n_dimension=2)
    lda.fit(x_train, y_train)
    y_test = lda.transform(x_test)
    assert y_test.shape == (m, 2), f"expect y_test.shape={(m, 2)}, get y_test.shape={y_test.shape}"
    assert y_test.dtype == np.float32, f"expect y_test.dtype={np.float32}, get y_test.dtype={y_test.dtype}"


@safe_run("检查Ridge2的输入输出格式")
def classifier_2_sanity_check(classifier_2_class: type[RidgeMeta]):
    rng = np.random.default_rng()
    n = 2
    (m,) = rng.integers(512, 1024, size=1, dtype=np.int32)
    (d,) = rng.integers(512, 1024, size=1, dtype=np.int32)
    x_train = rng.random(size=(m, d), dtype=np.float32)
    y_train = rng.integers(0, n, size=(m,), dtype=np.int32)
    x_test = rng.random(size=(m, d), dtype=np.float32)
    classifier_2 = classifier_2_class(Lambda=1.0)
    classifier_2.fit(x_train, y_train)
    y_test = classifier_2.predict(x_test)
    assert y_test.shape == (m,), f"expect y_test.shape={(m,)}, get y_test.shape={y_test.shape}"
    assert y_test.dtype == np.int32, f"expect y_test.dtype={np.int32}, get y_test.dtype={y_test.dtype}"
    assert y_test.min() >= 0, f"y_test.min()>=0, get y_test.min()={y_test.min()}"
    assert y_test.max() < 2, f"y_test.max()<{2}, get y_test.max()={y_test.max()}"


@safe_run("检查RidgeN的输入输出格式")
def classifier_n_sanity_check(classifier_n_class: type[RidgeMeta]):
    rng = np.random.default_rng()
    (n,) = rng.integers(8, 16, size=1, dtype=np.int32)
    (m,) = rng.integers(512, 1024, size=1, dtype=np.int32)
    (d,) = rng.integers(512, 1024, size=1, dtype=np.int32)
    x_train = rng.random(size=(m, d), dtype=np.float32)
    y_train = rng.integers(0, n, size=(m,), dtype=np.int32)
    x_test = rng.random(size=(m, d), dtype=np.float32)
    classifier_n = classifier_n_class(Lambda=1.0)
    classifier_n.fit(x_train, y_train)
    y_test = classifier_n.predict(x_test)
    assert y_test.shape == (m,), f"expect y_test.shape={(m,)}, get y_test.shape={y_test.shape}"
    assert y_test.dtype == np.int32, f"expect y_test.dtype={np.int32}, get y_test.dtype={y_test.dtype}"
    assert y_test.min() >= 0, f"y_test.min()>=0, get y_test.min()={y_test.min()}"
    assert y_test.max() < n, f"y_test.max()<{n}, get y_test.max()={y_test.max()}"
