import numpy as np
import matplotlib.pyplot as plt

# 设置网格细胞布局参数
grid_spacing = 6  # 网格细胞的活动热点间隔（对应周期性）
num_rows = 5  # 蜂窝形排列的行数
num_cols = 5  # 蜂窝形排列的列数

# 生成蜂窝形排列的细胞位置
cell_positions = []
for i in range(num_rows):
    for j in range(num_cols):
        x = j * grid_spacing
        y = i * grid_spacing * np.sqrt(3) / 2
        if i % 2 == 1:  # 奇数行水平偏移
            x += grid_spacing / 2
        cell_positions.append((x, y))

# 绘制网格细胞活动热点的函数，细胞圈划分为三个层次
def plot_grid_cells(cell_positions, spacing):
    for pos in cell_positions:
        # 设置每个层次的半径
        radii = [spacing / 6, spacing / 3, spacing / 2]  # 三个层次
        colors = ['yellow', 'orange', 'red']  # 每个层次的颜色
        alphas = [0.3, 0.5, 0.7]  # 每个层次的透明度
        
        # 绘制每个层次
        for i in range(3):
            circle = plt.Circle(pos, radii[i], color=colors[i], alpha=alphas[i])
            plt.gca().add_patch(circle)
        
        # 标记中心点
        plt.plot(*pos, 'ro')  # 中心点标记为红色

# 计算信号强度：考虑每个细胞贡献的信号强度
def calculate_signal_strength(start_position, angle, cell_positions, spacing, distance_thresholds):
    total_signal = 0
    angle_rad = np.deg2rad(angle)

    for pos in cell_positions:
        rel_x, rel_y = pos[0] - start_position[0], pos[1] - start_position[1]
        A = -np.tan(angle_rad) if np.cos(angle_rad) != 0 else 0
        B = 1
        C = 0
        if np.cos(angle_rad) != 0:
            line_distance = np.abs(A * rel_x + B * rel_y + C) / np.sqrt(A**2 + B**2)
        else:
            line_distance = abs(rel_x)

        if line_distance <= spacing / 2:
            signal = 1 / (line_distance + 1)  # 信号强度与距离成反比
            total_signal += signal

    return total_signal

# 绘制网格中的行进路径
def plot_traversal_path(start_pos, angle, steps=20, step_size=1, cell_positions=None, grid_spacing=None, distance_thresholds=None):
    angle_rad = np.deg2rad(angle)
    x_path = [start_pos[0]]
    y_path = [start_pos[1]]
    signal_strengths = []
    
    for i in range(steps):
        x_new = x_path[-1] + step_size * np.cos(angle_rad)
        y_new = y_path[-1] + step_size * np.sin(angle_rad)
        x_path.append(x_new)
        y_path.append(y_new)
        total_signal = calculate_signal_strength(start_pos, angle, cell_positions, grid_spacing, distance_thresholds)
        signal_strengths.append(total_signal)
    
    plt.plot(x_path, y_path, label=f"Angle {angle}°")
    return signal_strengths, x_path, y_path

# 绘制信号强度的周期变化
def plot_sine_wave_periodic(start_position, cell_positions, grid_spacing, distance_thresholds):
    phi = np.linspace(0, 360, 1000)
    signal_strengths = [
        calculate_signal_strength(start_position, angle, cell_positions, grid_spacing, distance_thresholds)
        for angle in phi
    ]
    plt.plot(phi, signal_strengths, color='b', linewidth=2, label="Periodic Signal")
    plt.xlabel("Running direction (°)")
    plt.ylabel("Signal strength")
    plt.title("Periodic Signal of Grid Cells")
    plt.legend()
    plt.grid()

# 创建子图
fig, ax = plt.subplots(1, 2, figsize=(14, 6))

# 左图：绘制蜂窝网格细胞布局和行进路径
plt.sca(ax[0])
plt.xlim(-10, grid_spacing * num_cols)
plt.ylim(-10, grid_spacing * num_rows * np.sqrt(3) / 2)
plt.gca().set_aspect('equal')
plot_grid_cells(cell_positions, grid_spacing)
start_position = (12, 10)
distance_thresholds = [grid_spacing / 6, grid_spacing / 3, grid_spacing / 2]
angles = [0, 45, 90, 135, 180, 225, 270, 315]
for angle in angles:
    plot_traversal_path(start_position, angle, steps=20, step_size=1, cell_positions=cell_positions, grid_spacing=grid_spacing, distance_thresholds=distance_thresholds)
plt.title("Traversal Paths Across Hexagonal Grid Cells")
plt.legend()

# 右图：绘制信号强度的周期变化
plt.sca(ax[1])
plot_sine_wave_periodic(start_position, cell_positions, grid_spacing, distance_thresholds)

plt.tight_layout()
output_path = "./hexagonal_grid.png"
plt.savefig(output_path, dpi=300, bbox_inches='tight')
plt.show()
