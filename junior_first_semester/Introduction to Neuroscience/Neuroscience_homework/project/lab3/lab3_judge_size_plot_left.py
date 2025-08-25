import pandas as pd
import matplotlib.pyplot as plt
import os
import re

# 输入昵称，昵称为全英文字母或 "all"
nickname = input("请输入昵称（英文字母）或输入 'all' 以处理所有文件: ").strip()

# 定义数据目录和文件匹配模式
data_dir = './data_left'
results_dir = './results_left'
os.makedirs(results_dir, exist_ok=True)  # 确保结果目录存在
file_pattern = re.compile(rf"{nickname}_\d{{2}}_results\.csv") if nickname != "all" else re.compile(r".+_\d{2}_results\.csv")

# 收集所有匹配文件
files_to_process = [os.path.join(data_dir, f) for f in os.listdir(data_dir) if file_pattern.match(f)]

# 检查是否找到文件
if not files_to_process:
    print("未找到符合条件的文件。")
else:
    # 读取并合并所有文件的数据
    data = pd.concat([pd.read_csv(file) for file in files_to_process], ignore_index=True)

    # 按 Ratio_r 分组并计算均值
    mean_data = data.groupby('Ratio_r').agg(
        Mean_Reaction_Time=('Reaction_Time', 'mean'),
        Accuracy=('Correct', 'mean')
    ).reset_index()
    # 计算总准确率和总平均反应时间（Ratio_r为0的数据不计算在内）
    total_accuracy = data[data['Ratio_r'] != 0]['Correct'].mean()
    total_mean_reaction_time = data['Reaction_Time'].mean()

    # 计算选右的概率
    data['Chose_Right'] = data['Response'].isin(['right', 'j'])  # 将选“右”或按键“j”标记为 True，其他为 False
    prob_right = data.groupby('Ratio_r')['Chose_Right'].mean().reset_index()  # 计算每个 Ratio_r 下选右的概率
    
    # 将选右的概率合并到 mean_data 中
    mean_data = pd.merge(mean_data, prob_right, on='Ratio_r', how='left')
    mean_data.rename(columns={'Chose_Right': 'Probability_of_Choosing_Right'}, inplace=True)

    # 绘制反应时间随 Ratio_r 的变化图
    plt.figure(figsize=(10, 6))
    plt.plot(mean_data['Ratio_r'], mean_data['Mean_Reaction_Time'], marker='o', linestyle='-', color='b', label='Mean Reaction Time')
    plt.title('Mean Reaction Time vs Ratio r')
    plt.xlabel('Ratio r')
    plt.ylabel('Mean Reaction Time (seconds)')
    plt.grid(True)
    plt.xticks(mean_data['Ratio_r'].unique())
    plt.legend()
    
    # 保存反应时间图到 results 目录
    plot_filename = os.path.join(results_dir, f"{nickname}_plot_time.png")
    plt.savefig(plot_filename)
    print(f"反应时间图已保存为: {plot_filename}")

    # 绘制选右的概率随 Ratio_r 的变化图
    plt.figure(figsize=(10, 6))
    plt.plot(prob_right['Ratio_r'], prob_right['Chose_Right'], marker='o', linestyle='-', color='g', label='Probability of Choosing Right')
    plt.title('Probability of Choosing Right vs Ratio r')
    plt.xlabel('Ratio r')
    plt.ylabel('Probability of Choosing Right')
    plt.grid(True)
    plt.xticks(prob_right['Ratio_r'].unique())
    plt.legend()

    # 保存选右的概率图到 results 目录
    prob_plot_filename = os.path.join(results_dir, f"{nickname}_plot_prob.png")
    plt.savefig(prob_plot_filename)
    print(f"选右的概率图已保存为: {prob_plot_filename}")
    
    # 打印统计结果
    print("Ratio_r  Mean_Reaction_Time  Accuracy")
    print(mean_data)

    # 打印总准确率和总平均反应时间
    print(f"\nTotal Accuracy: {total_accuracy:.2f}")
    print(f"Total Mean Reaction Time: {total_mean_reaction_time:.2f} seconds")

    # 将统计数据保存为 CSV 文件到 results 目录
    statistic_filename = os.path.join(results_dir, f"{nickname}_statistic.csv")
    mean_data.to_csv(statistic_filename, index=False)
    with open(statistic_filename, 'a') as f:
        f.write(f"\nTotal Accuracy: {total_accuracy:.2f}\nTotal Mean Reaction Time: {total_mean_reaction_time:.2f} seconds")
    print(f"统计数据已保存为: {statistic_filename}")
    