import numpy as np
import matplotlib.pyplot as plt
import csv


# --- DDM Default Parameters ---
total_trials = 1000           # Total number of trials for each ratio_level
drift_rate = 2.25              # Drift rate (evidence accumulation rate)
sigma = 0.2                  # Noise standard deviation
B = 0.1                      # Decision boundary
time_non_decision = 0.2       # Non-decision time
dt = 0.001                    # Time step
max_t = 2.0                   # Maximum simulation time
max_t_steps = int(max_t / dt) # Maximum number of time steps


# --- DDM Simulation Function (for a single trial) ---
def simulate_ddm(ratio, drift_rate, sigma, B, dt, max_t_steps, time_non_decision):
    evidence = 0.0  # Initial decision variable
    evidence_store = np.zeros(max_t_steps)
    choice = 0  # Decision: 0 for no decision, 1 for right, -1 for left
    t_step = 0  # Time step
    reaction_time = 0   # Reaction time

    while abs(evidence) < B and t_step < max_t_steps:
        dx = drift_rate * ratio * dt + sigma * np.random.randn() * np.sqrt(dt)  # Euler-Maruyama integration
        evidence += dx
        evidence_store[t_step] = evidence
        t_step += 1
    
    if evidence >= B:
        choice = 1  # Decision: Right boundary
        evidence_store[t_step:max_t_steps] = B
    elif evidence <= -B:
        choice = -1  # Decision: Left boundary
        evidence_store[t_step:max_t_steps] = -B
    
    reaction_time = t_step * dt + time_non_decision  # Reaction time
    
    if choice == 0: # If no decision is made, choose based on the accumulated evidence
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
def ddm_fitting(drift_rate, sigma, B):
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
        reaction_time, evidence_store, choice = simulate_ddm(ratio, drift_rate, sigma, B, dt, max_t_steps, time_non_decision)
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
    drift_rate_grid = np.around(np.linspace(1.3, 2.5, 5), decimals=2)
    sigma_grid = np.around(np.linspace(0.2, 0.3, 5), decimals=2)
    B_grid = np.around(np.linspace(0.05, 0.15, 5), decimals=2)
    
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
                mse, acc, p_right, reaction_time_mean = ddm_fitting(drift_rate, sigma, B)
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


# --- Plotting Functions ---
def plot_result(ratios, ddm_acc, experiment_acc, ddm_p_right, experiment_p_right, ddm_reaction_time_mean, experiment_reaction_time_mean, best_drift_rate, best_sigma, best_B, best_mse):
    # Plot
    plt.figure(figsize=(12, 12))

    # Overall title with MSE
    plt.suptitle(f"DDM Fitting Results (Best MSE of RT-R = {np.round(best_mse, 6)})", fontsize=16)

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
    plt.savefig("lab4_ddm_fitting_results_noised.png")  # 保存图像文件，格式为PNG，可以根据需要修改格式
    plt.show()


# --- Plot Evidence Accumulation ---
def plot_evidence_accumulation(ratios, drift_rate, sigma, B, dt, max_t_steps, time_non_decision):
    plt.figure(figsize=(10, 6))
    
    colors = ['blue', 'orange', 'green', 'red', 'yellow']  # 不同颜色表示不同的 ratio
    max_reaction_index = 0  # 记录达到边界的最大索引
    
    for idx, ratio in enumerate(ratios):
        # Run a single trial for each ratio
        reaction_time, evidence_store, choice = simulate_ddm(ratio, drift_rate, sigma, B, dt, max_t_steps, time_non_decision)
        
        # 找到 evidence_store 首次达到边界 B 或 -B 的索引位置
        boundary_index = next((i for i, value in enumerate(evidence_store) if abs(value) >= B), len(evidence_store) - 1)
        max_reaction_index = max(max_reaction_index, boundary_index)
        
        # 时间轴和证据积累过程截断在边界处，并增加少量余量
        plot_index = boundary_index + 10  # 增加10个步长的余量
        time_axis = np.arange(0, plot_index * dt, dt)
        
        plt.plot(time_axis, evidence_store[:plot_index], color=colors[idx], label=f"Ratio={ratio}, Choice={'Right' if choice == 1 else 'Left'}")
    
    # 根据所有曲线的最大反应时间来限制 x 轴范围
    max_plot_time = (max_reaction_index + 10) * dt
    plt.xlim(0, max_plot_time)
    
    # 绘制决策边界线
    plt.axhline(y=B, color="green", linestyle="--", label="Decision Boundary (Right)")
    plt.axhline(y=-B, color="red", linestyle="--", label="Decision Boundary (Left)")
    
    plt.xlabel("Time (s)")
    plt.ylabel("Evidence")
    plt.title("Evidence Accumulation for Different Ratios")
    plt.legend()
    
    # Save the figure
    plt.savefig("lab4_ddm_visualization_of_accumulation_noised")
    plt.show() 
    

if __name__ == '__main__':
    
    ratios, experiment_reaction_time_mean, experiment_acc, experiment_p_right = load_experiment_data("./noised_results/all_statistic.csv")

    ddm_acc, ddm_p_right, ddm_reaction_time_mean, best_drift_rate, best_sigma, best_B, best_mse = grid_search()
    
    plot_result(ratios, ddm_acc, experiment_acc, ddm_p_right, experiment_p_right, ddm_reaction_time_mean, experiment_reaction_time_mean, best_drift_rate, best_sigma, best_B, best_mse)
    
    plot_evidence_accumulation([0, 0.08, -0.08, 0.32, -0.32], best_drift_rate, best_sigma, best_B, dt, max_t_steps, time_non_decision)
