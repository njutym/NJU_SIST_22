import subprocess
import time
import numpy as np
import matplotlib.pyplot as plt

# 可执行文件列表
executables = ['main']
# 参数列表,为线程个数
args = [1, 2, 4, 8, 16, 32, 64]
# args = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
# 存储结果的字典
results = {exe: {arg: [] for arg in args} for exe in executables}

# 运行可执行文件并记录时间
for exe in executables:
    for arg in args:
        times = []
        for _ in range(5):  # 对每个参数运行5次
            start = time.time()
            subprocess.run([f"./{exe}", "-t", str(arg)])
            end = time.time()
            times.append(end - start)
        results[exe][arg] = times

# 将数据写入文件
with open('./running_time.txt', 'w') as file:
    for exe in executables:
        for arg in args:
            for time_val in results[exe][arg]:
                file.write(f"{exe} {arg} {time_val}\n")
                
# 读取数据
with open('running_time.txt', 'r') as file:
    lines = file.readlines()

# 解析数据
results = {}
for line in lines:
    exe, arg, time_val = line.split()  # 将每行数据分割成可执行文件、参数和时间
    time_val = float(time_val)  # 将时间值转换为浮点数
    if exe not in results:
        results[exe] = {}
    if int(arg) not in results[exe]:
        results[exe][int(arg)] = []
    results[exe][int(arg)].append(time_val)

# 计算中位数和标准差
medians = {}
errors = {}
for exe in results:
    medians[exe] = []
    errors[exe] = []
    for arg in results[exe]:
        medians[exe].append(np.median(results[exe][arg]))
        errors[exe].append(np.std(results[exe][arg]))

# 绘图
plt.figure(figsize=(10, 6))
for exe in results:
    args = list(results[exe].keys())
    plt.errorbar(args, medians[exe], yerr=errors[exe], capsize=5, label=exe)

plt.xlabel('Numbers of threads')
plt.ylabel('Time (s)')
plt.title('Execution time of different numbers of threads')
plt.xticks(args)
plt.legend()
plt.grid(True)
plt.savefig(f'./result/img/exeTime_{time.time()}.png')
plt.clf()
