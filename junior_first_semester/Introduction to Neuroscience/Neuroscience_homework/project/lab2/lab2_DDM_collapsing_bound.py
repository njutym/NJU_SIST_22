import numpy as np
import matplotlib.pyplot as plt
import csv


# --- DDM Default Parameters ---
total_trials = 1000           # Total number of trials for each ratio_level
drift_rate = 1.5              # Drift rate (evidence accumulation rate)
sigma = 0.12                  # Noise standard deviation
B = 0.08                      # Decision boundary
decay_rate = 1.5              # Decay rate for the collapsing boundary
time_non_decision = 0.2       # Non-decision time
dt = 0.001                    # Time step
max_t = 2.0                   # Maximum simulation time
max_t_steps = int(max_t / dt) # Maximum number of time steps


# --- DDM Simulation Function (for a single trial) ---
def simulate_ddm_collapsing_bound(ratio, drift_rate, sigma, B0, dt, max_t_steps, time_non_decision, decay_rate):
    evidence = 0.0  # Initial decision variable
    evidence_store = np.zeros(max_t_steps)
    choice = 0  # Decision: 0 for no decision, 1 for right, -1 for left
    t_step = 0  # Time step
    reaction_time = 0   # Reaction time

    while abs(evidence) < B0 and t_step < max_t_steps:
        # Calculate the current threshold B(t) as exponentially decaying function of time
        B_t = B0 * np.exp(-decay_rate * t_step * dt)
        
        # Check if the evidence crosses the decaying threshold
        dx = drift_rate * ratio * dt + sigma * np.random.randn() * np.sqrt(dt)  # Euler-Maruyama integration
        evidence += dx
        evidence_store[t_step] = evidence
        t_step += 1
        
        if abs(evidence) >= B_t:  # Boundary condition with collapsing threshold
            break
    
    if evidence >= 0:
        choice = 1  # Decision: Right boundary
        evidence_store[t_step:max_t_steps] = evidence
    elif evidence <= 0:
        choice = -1  # Decision: Left boundary
        evidence_store[t_step:max_t_steps] = evidence
    
    reaction_time = t_step * dt + time_non_decision  # Reaction time
    
    if choice == 0:  # If no decision is made, choose based on the accumulated evidence
        choice = np.sign(evidence)
    
    return reaction_time, evidence_store, choice  # 1 for right boundary, -1 for left


# --- Load Experiment Data from CSV File ---
def load_experiment_data(filepath):
    ratios = []
    experiment_reaction_time_mean = []
    experiment_acc = []
    experiment_p_right = []
    
    with open(filepath, newline='') as csvfile:
        csvreader = csv.reader(csvfile)
        next(csvreader)  # Skip the header line if present
        
        reaction_times = []  # To store the raw reaction times for normalization
        
        # First pass to collect all reaction times
        for row in csvreader:
            if len(row) != 4:  # Ensure there are exactly 4 columns in the row
                print(f"Skipping row due to incorrect column count: {row}")
                continue  # Skip the row if it doesn't have exactly 4 elements
            
            try:
                ratio = float(row[0])
                mean_reaction_time = float(row[1])
                accuracy = float(row[2])
                probability_of_right = float(row[3])

                ratios.append(ratio)
                experiment_acc.append(accuracy)
                experiment_p_right.append(probability_of_right)
                
                # Collect reaction times for normalization
                reaction_times.append(mean_reaction_time)
            
            except ValueError as e:
                print(f"Skipping row due to invalid data: {row}. Error: {e}")
                
        for mean_reaction_time in reaction_times:
            experiment_reaction_time_mean.append(mean_reaction_time)
            
    return ratios, experiment_reaction_time_mean, experiment_acc, experiment_p_right


# --- DDM Fitting Function(for a certain set of parameters) ---
def ddm_fitting(drift_rate, sigma, B,decay_rate):
    # Clear previous simulation data
    ddm_choices = [[] for _ in range(len(ratios))]
    ddm_reaction_time_all = [[] for _ in range(len(ratios))]
    ddm_acc = []
    ddm_p_right = []
    ddm_reaction_time_mean = []

    # Run DDM fitting with the current parameter set
    for _ in range(total_trials):
        i = np.random.randint(0, len(ratios))
        ratio = ratios[i]
        reaction_time, evidence_store, choice = simulate_ddm_collapsing_bound(ratio, drift_rate, sigma, B, dt, max_t_steps, time_non_decision,decay_rate)
        ddm_choices[i].append(choice)
        ddm_reaction_time_all[i].append(reaction_time)

    for i in range(len(ratios)):
        correct_choices = 0
        total_choices = len(ddm_choices[i])

        ddm_p_right.append(ddm_choices[i].count(1) / total_choices if total_choices > 0 else 0)
        ddm_reaction_time_mean.append(np.mean(ddm_reaction_time_all[i]) if total_choices > 0 else 0)

        for choice in ddm_choices[i]:
            if choice == np.sign(ratios[i]):
                correct_choices += 1
        ddm_acc.append(correct_choices / total_choices if total_choices > 0 else 0)

    # Calculate MSE between DDM reaction time mean and experiment reaction time mean
    mse = np.mean((np.array(ddm_reaction_time_mean) - np.array(experiment_reaction_time_mean)) ** 2)
    return mse, ddm_acc, ddm_p_right, ddm_reaction_time_mean


# --- Grid Search Function ---
def grid_search():
    # Parameter grids
    drift_rate_grid = np.around(np.linspace(1.5, 2.5, 5), decimals=2)
    sigma_grid = np.around(np.linspace(0.1, 0.2, 5), decimals=2)
    B_grid = np.around(np.linspace(0.01, 0.1, 5), decimals=2)  
    
    best_mse = float('inf')  # Initialize best MSE to a very high value
    best_drift_rate = None
    best_sigma = None
    best_B = None
    ddm_acc = []  # DDM data: accuracy
    ddm_p_right = []  # DDM data: proportion of right choices
    ddm_reaction_time_mean = []  # DDM data: mean reaction time
    for drift_rate in drift_rate_grid:
        for sigma in sigma_grid:
            for B in B_grid:
                mse, acc, p_right, reaction_time_mean = ddm_fitting(drift_rate, sigma, B,decay_rate)
                print("Drift Rate: {}, Sigma: {}, B: {}, MSE: {}, Best MSE: {}".format(drift_rate, sigma, B, mse, best_mse))
                if mse < best_mse:
                    print("New Best MSE found: {}".format(mse))
                    best_mse = mse
                    best_drift_rate = drift_rate
                    best_sigma = sigma
                    best_B = B
                    ddm_acc = acc
                    ddm_p_right = p_right
                    ddm_reaction_time_mean = reaction_time_mean
    
    print("Best Drift Rate: {}, Best Sigma: {}, Best B: {}, Best MSE: {}".format(best_drift_rate, best_sigma, best_B, best_mse))
    
    # Return the results
    return ddm_acc, ddm_p_right, ddm_reaction_time_mean, best_drift_rate, best_sigma, best_B, best_mse


# --- Plotting Function ---
def plot_result(ratios, ddm_acc, experiment_acc, ddm_p_right, experiment_p_right, ddm_reaction_time_mean, experiment_reaction_time_mean, best_drift_rate, best_sigma, best_B, best_mse):
    # Plot
    plt.figure(figsize=(12, 12))

    # Overall title with MSE
    plt.suptitle(f"DDM Collasping Bound Fitting Results (Best MSE = {np.round(best_mse, 6)})", fontsize=16)

    # Accuracy comparison
    plt.subplot(3, 1, 1)
    plt.plot(ratios, ddm_acc, marker='o', label=f'DDM Model (drift_rate={best_drift_rate}, sigma={best_sigma}, B={best_B})', color='blue')
    plt.plot(ratios, experiment_acc, marker='x', label='Experiment Data', color='red')
    plt.title("Accuracy Comparison")
    plt.xlabel("Ratios")
    plt.ylabel("Accuracy")
    plt.legend()

    # Right choice comparison
    plt.subplot(3, 1, 2)
    plt.plot(ratios, ddm_p_right, marker='o', label=f'DDM Model (drift_rate={best_drift_rate}, sigma={best_sigma}, B={best_B})', color='blue')
    plt.plot(ratios, experiment_p_right, marker='x', label='Experiment Data', color='red')
    plt.title("Psychometric Curve Comparison")
    plt.xlabel("Ratios")
    plt.ylabel("Right Choices")
    plt.legend()

    # Reaction time comparison
    plt.subplot(3, 1, 3)
    plt.plot(ratios, ddm_reaction_time_mean, marker='o', label=f'DDM Model (drift_rate={best_drift_rate}, sigma={best_sigma}, B={best_B})', color='blue')
    plt.plot(ratios, experiment_reaction_time_mean, marker='x', label='Experiment Data', color='red')
    plt.title("Chronometric Curve Comparison")
    plt.xlabel("Ratios")
    plt.ylabel("Reaction Time")
    plt.legend()

    plt.tight_layout(rect=[0, 0, 1, 0.96])  # Adjust layout to fit the overall title
    
    # Save the plot to a file
    plt.savefig("lab2_ddm_collapsing_bound_fitting_results.png")  # 保存图像文件，格式为PNG，可以根据需要修改格式
    plt.show()


# --- Plotting Function for Evidence Accumulation ---
def plot_evidence_accumulation(ratios, drift_rate, sigma, B, dt, max_t_steps, time_non_decision, decay_rate):
    plt.figure(figsize=(10, 6))
    
    colors = ['blue', 'orange', 'green', 'red', 'yellow']  # 不同颜色表示不同的 ratio
    max_reaction_time = 0  # 记录达到边界的最大反应时间
    
    for idx, ratio in enumerate(ratios):
        # Run a single trial for each ratio
        reaction_time, evidence_store, choice = simulate_ddm_collapsing_bound(ratio, drift_rate, sigma, B, dt, max_t_steps, time_non_decision, decay_rate)
        
        # 直接使用 reaction_time 来更新 max_reaction_time
        max_reaction_time = max(max_reaction_time, reaction_time)
        
        # 找到反应时间的位置，并截断证据积累曲线
        plot_index = int(reaction_time / dt) + 10  # 增加10个步长的余量
        plot_index = min(plot_index, len(evidence_store))  # 确保 plot_index 不超过 evidence_store 的长度
        time_axis = np.arange(0, plot_index * dt, dt)
        
        # 绘制证据积累过程（只绘制到达到边界的时刻）
        plt.plot(time_axis[:plot_index], evidence_store[:plot_index], color=colors[idx], label=f"Ratio={ratio}, Choice={'Right' if choice == 1 else 'Left'}")
    
    # 计算动态的正负边界（相同的边界）
    extended_time = int(max_reaction_time / dt) + 20  # 多绘制20个时间步长，确保边界延长显示
    time_axis_extended = np.arange(0, extended_time * dt, dt)
    
    dynamic_boundary_pos = [B * np.exp(-decay_rate * t_step * dt) for t_step in range(extended_time)]
    dynamic_boundary_neg = [-B * np.exp(-decay_rate * t_step * dt) for t_step in range(extended_time)]
    
    # 绘制动态的正负边界（相同的衰减）
    plt.plot(time_axis_extended, dynamic_boundary_pos, color="green", linestyle='--', label="Dynamic Boundary (Positive)")
    plt.plot(time_axis_extended, dynamic_boundary_neg, color="red", linestyle='--', label="Dynamic Boundary (Negative)")
    
    # 绘制 x 轴范围，确保比最大反应时间稍长
    max_plot_time = max_reaction_time + dt * 10  # 多绘制 10 个时间步长
    plt.xlim(0, max_plot_time)
    
    plt.xlabel("Time (s)")
    plt.ylabel("Evidence")
    plt.title("Evidence Accumulation for Different Ratios")
    plt.legend()
    
    # Save the figure
    plt.savefig("lab2_ddm_cb_visualization_of_accumulation")
    plt.show()


if __name__ == '__main__':
    
    ratios, experiment_reaction_time_mean, experiment_acc, experiment_p_right = load_experiment_data("./results/all_statistic.csv")

    ddm_acc, ddm_p_right, ddm_reaction_time_mean, best_drift_rate, best_sigma, best_B, best_mse = grid_search()
    
    plot_result(ratios, ddm_acc, experiment_acc, ddm_p_right, experiment_p_right, ddm_reaction_time_mean, experiment_reaction_time_mean, best_drift_rate, best_sigma, best_B, best_mse)

    plot_evidence_accumulation([0, 0.08, -0.08, 0.32, -0.32], best_drift_rate, best_sigma, best_B, dt, max_t_steps, time_non_decision,decay_rate)