import numpy as np
import matplotlib.pyplot as plt
import csv
import matplotlib.cm as cm

# --- DDM Default Parameters ---
total_trials = 2000            # Total number of trials for each ratio_level
drift_rate = 1.5              # Drift rate (evidence accumulation rate)
sigma = 0.14                  # Noise standard deviation
B = 0.1                       # Decision boundary
time_non_decision = 0.3       # Non-decision time
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
        i = np.random.randint(0, len(ratios)) # 随机生成一个整数
        ratio = ratios[i]
        reaction_time, evidence_store, choice = simulate_ddm(ratio, drift_rate, sigma, B, dt, max_t_steps, time_non_decision)
        ddm_choices[i].append(choice)
        ddm_reaction_time_all[i].append(reaction_time)

    for i in range(len(ratios)):
        correct_choices = 0
        total_choices = len(ddm_choices[i])

        ddm_p_right.append(ddm_choices[i].count(1) / total_choices if total_choices > 0 else 0) # 选右的比例
        ddm_reaction_time_mean.append(np.mean(ddm_reaction_time_all[i]) if total_choices > 0 else 0)

        for choice in ddm_choices[i]:
            if choice == np.sign(ratios[i]):
                correct_choices += 1
        ddm_acc.append(correct_choices / total_choices if total_choices > 0 else 0)

    # Calculate MSE between DDM reaction time mean and experiment reaction time mean
    mse = np.mean((np.array(ddm_reaction_time_mean) - np.array(experiment_reaction_time_mean)) ** 2)
    return mse, ddm_acc, ddm_p_right, ddm_reaction_time_mean

def plot_result_list2(ratios, best_ddm_acc, ddm_acc_1, ddm_acc_2, experiment_acc, best_ddm_p_right, ddm_p_right_1, ddm_p_right_2, experiment_p_right, best_ddm_reaction_time_mean, ddm_reaction_time_mean_1, ddm_reaction_time_mean_2, experiment_reaction_time_mean, best_drift_rate, drift_rate_1, drift_rate_2, best_sigma, sigma_1, sigma_2, best_B, B_1, B_2):
    # Plot
    plt.figure(figsize=(12, 12))

    # Accuracy comparison
    plt.subplot(3, 1, 1)
    plt.plot(ratios, best_ddm_acc, marker='o', label=f'DDM Model (drift_rate={best_drift_rate}, sigma={best_sigma}, B={best_B})', color='blue')
    plt.plot(ratios, ddm_acc_1, marker='o', label=f'DDM Model (drift_rate={drift_rate_1}, sigma={sigma_1}, B={B_1})', color='green', linestyle='--')
    plt.plot(ratios, ddm_acc_2, marker='o', label=f'DDM Model (drift_rate={drift_rate_2}, sigma={sigma_2}, B={B_2})', color='orange', linestyle='--')
    plt.plot(ratios, experiment_acc, marker='x', label='Experiment Data', color='red')
    plt.title("Accuracy Comparison: DDM vs Experiment")
    plt.xlabel("Ratios")
    plt.ylabel("Accuracy")
    plt.legend()

    # Right choice comparison
    plt.subplot(3, 1, 2)
    plt.plot(ratios, best_ddm_p_right, marker='o', label=f'DDM Model (drift_rate={best_drift_rate}, sigma={best_sigma}, B={best_B})', color='blue')
    plt.plot(ratios, ddm_p_right_1, marker='o', label=f'DDM Model (drift_rate={drift_rate_1}, sigma={sigma_1}, B={B_1})', color='green', linestyle='--')
    plt.plot(ratios, ddm_p_right_2, marker='o', label=f'DDM Model (drift_rate={drift_rate_2}, sigma={sigma_2}, B={B_2})', color='orange', linestyle='--')
    plt.plot(ratios, experiment_p_right, marker='x', label='Experiment Data', color='red')
    plt.title("Psychometric Curve Comparison")
    plt.xlabel("Ratios")
    plt.ylabel("Right Choices")
    plt.legend()

    # Reaction time comparison
    plt.subplot(3, 1, 3)
    plt.plot(ratios, best_ddm_reaction_time_mean, marker='o', label=f'DDM Model (drift_rate={best_drift_rate}, sigma={best_sigma}, B={best_B})', color='blue')
    plt.plot(ratios, ddm_reaction_time_mean_1, marker='o', label=f'DDM Model (drift_rate={drift_rate_1}, sigma={sigma_1}, B={B_1})', color='green', linestyle='--')
    plt.plot(ratios, ddm_reaction_time_mean_2, marker='o', label=f'DDM Model (drift_rate={drift_rate_2}, sigma={sigma_2}, B={B_2})', color='orange', linestyle='--')
    plt.plot(ratios, experiment_reaction_time_mean, marker='x', label='Experiment Data', color='red')
    plt.title("Chronometric Curve Comparison")
    plt.xlabel("Ratios")
    plt.ylabel("Reaction Time")
    plt.legend()

    plt.tight_layout()
    
    # 保存图像文件
    plt.savefig("lab1_multiple_parameters_effects.png")
    plt.show()


if __name__ == '__main__':
    ratios, experiment_reaction_time_mean, experiment_acc, experiment_p_right = load_experiment_data("./results/all_statistic.csv")

    best_drift_rate=1.5
    best_sigma=0.14
    best_B=0.1

    drift_rate_1=3.0
    sigma_1=0.02
    B_1=0.1

    drift_rate_2=0.5
    sigma_2=0.18
    B_2=0.1

    _, best_ddm_acc, best_ddm_p_right, best_ddm_reaction_time_mean = ddm_fitting(best_drift_rate, best_sigma, best_B)
    _, ddm_acc_1, ddm_p_right_1, ddm_reaction_time_mean_1 = ddm_fitting(drift_rate_1, sigma_1, B_1)
    _, ddm_acc_2, ddm_p_right_2, ddm_reaction_time_mean_2 = ddm_fitting(drift_rate_2, sigma_2, B_2)
    plot_result_list2(ratios, best_ddm_acc, ddm_acc_1, ddm_acc_2, experiment_acc, best_ddm_p_right, ddm_p_right_1, ddm_p_right_2, experiment_p_right, best_ddm_reaction_time_mean, ddm_reaction_time_mean_1, ddm_reaction_time_mean_2, experiment_reaction_time_mean, best_drift_rate, drift_rate_1, drift_rate_2, best_sigma, sigma_1, sigma_2, best_B, B_1, B_2)
