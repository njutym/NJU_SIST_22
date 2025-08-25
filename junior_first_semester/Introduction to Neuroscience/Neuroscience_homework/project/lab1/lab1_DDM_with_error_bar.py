# Description: Lab 1 - Drift Diffusion Model (DDM) with error bar(in reaction time plot)(the best parameter is known as drift_rate=1.5, sigma=0.14, B=0.1)

import numpy as np
import matplotlib.pyplot as plt
import csv

# --- DDM Default Parameters ---
total_trials = 1000           # Total number of trials for each ratio_level
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
    ddm_acc = [[] for _ in range(len(ratios))]  # 修改为存储每个trial的值
    ddm_p_right = [[] for _ in range(len(ratios))]  # 修改为存储每个trial的值
    ddm_reaction_time_mean = [[] for _ in range(len(ratios))]  # 修改为存储每个trial的值

    # Run DDM fitting with the current parameter set
    for _ in range(total_trials):
        for i in range(len(ratios)):
            ratio = ratios[i]
            reaction_time, evidence_store, choice = simulate_ddm(ratio, drift_rate, sigma, B, dt, max_t_steps, time_non_decision)
            ddm_choices[i].append(choice)
            ddm_reaction_time_all[i].append(reaction_time)

    for i in range(len(ratios)):
        correct_choices = 0
        total_choices = len(ddm_choices[i])

        ddm_p_right[i] = [1 if choice == 1 else 0 for choice in ddm_choices[i]]
        ddm_reaction_time_mean[i] = ddm_reaction_time_all[i]

        for choice in ddm_choices[i]:
            if choice == np.sign(ratios[i]):
                correct_choices += 1
        ddm_acc[i] = [1 if choice == np.sign(ratios[i]) else 0 for choice in ddm_choices[i]]

    # Calculate MSE between DDM reaction time mean and experiment reaction time mean
    ddm_reaction_time_mean_flat = [np.mean(rt_trials) for rt_trials in ddm_reaction_time_mean]
    mse = np.mean((np.array(ddm_reaction_time_mean_flat) - np.array(experiment_reaction_time_mean)) ** 2)
    return mse, ddm_acc, ddm_p_right, ddm_reaction_time_mean


# --- Grid Search Function ---
def grid_search():
    # Parameter grids
    # drift_rate_grid = np.around(np.linspace(0.5, 3.0, 6), decimals=2)
    # sigma_grid = np.around(np.linspace(0.02, 0.18, 5), decimals=2)
    # B_grid = np.around(np.linspace(0.1, 1.0, 6), decimals=2)  
    
    drift_rate_grid = np.around(np.linspace(1.5, 1.5, 1), decimals=2)
    sigma_grid = np.around(np.linspace(0.14, 0.14, 1), decimals=2)
    B_grid = np.around(np.linspace(0.1, 0.1, 1), decimals=2)  

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


# --- plot ---
def plot_result(ratios, ddm_acc, experiment_acc, ddm_p_right, experiment_p_right, ddm_reaction_time_mean, experiment_reaction_time_mean, best_drift_rate, best_sigma, best_B):
    plt.figure(figsize=(10, 5))  # 设置图形大小
    # # Accuracy comparison
    # plt.subplot(3, 1, 1)
    # ddm_acc_std = [np.std(acc_trials) / np.sqrt(len(acc_trials)) for acc_trials in ddm_acc]  # 计算每个r对应1000次trial的标准误差
    # ddm_acc_mean = [np.mean(acc_trials) for acc_trials in ddm_acc]
    # # plt.errorbar(ratios, ddm_acc_mean, yerr=ddm_acc_std, fmt='-o', label=f'DDM Model (drift_rate={best_drift_rate}, sigma={best_sigma}, B={best_B})', color='blue', capsize=5)
    # plt.plot(ratios, experiment_acc, marker='x', label='Experiment Data', color='red')
    # plt.title("Accuracy Comparison: DDM vs Experiment", fontsize=14)
    # plt.xlabel("Ratios", fontsize=12)
    # plt.ylabel("Accuracy", fontsize=12)
    # plt.xticks(fontsize=10)
    # plt.yticks(fontsize=10)
    # plt.legend()

    # # Right choice comparison
    # plt.subplot(3, 1, 2)
    # ddm_p_right_std = [np.std(p_trials) / np.sqrt(len(p_trials)) for p_trials in ddm_p_right]  # 计算每个r对应1000次trial的标准误差
    # ddm_p_right_mean = [np.mean(p_trials) for p_trials in ddm_p_right]
    # # plt.errorbar(ratios, ddm_p_right_mean, yerr=ddm_p_right_std, fmt='-o', label=f'DDM Model (drift_rate={best_drift_rate}, sigma={best_sigma}, B={best_B})', color='blue', capsize=5)
    # plt.plot(ratios, experiment_p_right, marker='x', label='Experiment Data', color='red')
    # plt.title("Psychometric Curve Comparison", fontsize=14)
    # plt.xlabel("Ratios", fontsize=12)
    # plt.ylabel("Right Choices", fontsize=12)
    # plt.xticks(fontsize=10)
    # plt.yticks(fontsize=10)
    # plt.legend()

    # Reaction time comparison
    # plt.subplot(3, 1, 3)
    ddm_reaction_time_std = [np.std(rt_trials) / np.sqrt(len(rt_trials)) for rt_trials in ddm_reaction_time_mean]  # 计算每个r对应1000次trial的标准误差
    ddm_reaction_time_mean_vals = [np.mean(rt_trials) for rt_trials in ddm_reaction_time_mean]
    plt.errorbar(ratios, ddm_reaction_time_mean_vals, yerr=ddm_reaction_time_std, fmt='-o', label=f'DDM Model (drift_rate={best_drift_rate}, sigma={best_sigma}, B={best_B})', color='blue', capsize=5)
    plt.plot(ratios, experiment_reaction_time_mean, marker='x', label='Experiment Data', color='red')
    plt.title("Chronometric Curve Comparison", fontsize=14)
    plt.xlabel("Ratios", fontsize=12)
    plt.ylabel("Reaction Time", fontsize=12)
    plt.legend()

    plt.tight_layout()

    # Save the plot to a file
    plt.savefig("lab1_ddm_fitting_results_with_error_bar.png")
    plt.show()
    
if __name__ == '__main__':
    
    ratios, experiment_reaction_time_mean, experiment_acc, experiment_p_right = load_experiment_data("./results/all_statistic.csv")

    ddm_acc, ddm_p_right, ddm_reaction_time_mean, best_drift_rate, best_sigma, best_B, best_mse = grid_search()
    
    plot_result(ratios, ddm_acc, experiment_acc, ddm_p_right, experiment_p_right, ddm_reaction_time_mean, experiment_reaction_time_mean, best_drift_rate, best_sigma, best_B)
