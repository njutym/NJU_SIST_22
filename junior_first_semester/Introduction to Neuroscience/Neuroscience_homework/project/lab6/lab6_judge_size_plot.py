import pandas as pd
import matplotlib.pyplot as plt
import os
import re

# 输入昵称，昵称为全英文字母或 "all"
nickname = input("请输入昵称（英文字母）或输入 'all' 以处理所有文件: ").strip()

# 定义数据目录和结果目录
data_dirs = ['./data_increase', './data_decrease']
results_dir = './results'
os.makedirs(results_dir, exist_ok=True)  # 确保结果目录存在

# 定义文件匹配模式
file_pattern = re.compile(rf"{nickname}_\d{{2}}_results\.csv") if nickname != "all" else re.compile(r".+_\d{2}_results\.csv")

# 用于存储所有数据
all_mean_data = []
all_prob_right_data = []
labels = [f'{nickname} abs(r) in increasing order', f'{nickname} abs(r) in decreasing order']

# 收集和处理每个目录中的数据
for data_dir, label in zip(data_dirs, labels):
    # 收集匹配的文件
    files_to_process = [os.path.join(data_dir, f) for f in os.listdir(data_dir) if file_pattern.match(f)]
    
    if files_to_process:
        # 读取并合并所有文件的数据
        data = pd.concat([pd.read_csv(file) for file in files_to_process], ignore_index=True)

        # 按 Ratio_r 分组并计算均值
        mean_data = data.groupby('Ratio_r').agg(
            Mean_Reaction_Time=('Reaction_Time', 'mean'),
            Accuracy=('Correct', 'mean')
        ).reset_index()
        all_mean_data.append((mean_data, label))

        # 计算选右的概率
        data['Chose_Right'] = data['Response'].isin(['right', 'j'])
        prob_right = data.groupby('Ratio_r')['Chose_Right'].mean().reset_index()
        all_prob_right_data.append((prob_right, label))

# 绘制两组数据的反应时间随 Ratio_r 的变化图
plt.figure(figsize=(10, 6))
for mean_data, label in all_mean_data:
    plt.plot(mean_data['Ratio_r'], mean_data['Mean_Reaction_Time'], marker='o', linestyle='-', label=f'{label} Mean Reaction Time')

plt.title('Mean Reaction Time vs Ratio r for data_small and data_large')
plt.xlabel('Ratio r')
plt.ylabel('Mean Reaction Time (seconds)')
plt.grid(True)
plt.xticks(all_mean_data[0][0]['Ratio_r'].unique())
plt.legend()

# 保存反应时间图到 results 目录
plot_filename = os.path.join(results_dir, f"{nickname}_combined_plot_time.png")
plt.savefig(plot_filename)
print(f"反应时间图已保存为: {plot_filename}")

# 绘制两组数据的选右概率随 Ratio_r 的变化图
plt.figure(figsize=(10, 6))
for prob_right, label in all_prob_right_data:
    plt.plot(prob_right['Ratio_r'], prob_right['Chose_Right'], marker='o', linestyle='-', label=f'{label} Probability of Choosing Right')

plt.title('Probability of Choosing Right vs Ratio r for data_small and data_large')
plt.xlabel('Ratio r')
plt.ylabel('Probability of Choosing Right')
plt.grid(True)
plt.xticks(all_prob_right_data[0][0]['Ratio_r'].unique())
plt.legend()

# 保存选右概率图到 results 目录
prob_plot_filename = os.path.join(results_dir, f"{nickname}_combined_plot_prob.png")
plt.savefig(prob_plot_filename)
print(f"选右的概率图已保存为: {prob_plot_filename}")

