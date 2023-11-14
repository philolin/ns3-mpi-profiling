#!/usr/bin/python3
import subprocess
import matplotlib.pyplot as plt
import os
import math
from tqdm import tqdm

def run_simulation(num_cores, n_pods):
    command = f'cd ns-allinone-3.40/ns-3.40/; /usr/bin/time -f "%e" ./ns3 run scratch/fat-tree-animation-mpi.cc --command-template="mpiexec -np {num_cores} %s" -- --nPods={n_pods}'
    # print(command)
    result = subprocess.run(command, shell=True, stderr=subprocess.PIPE, universal_newlines=True)
    # Extract the real time from the output
    elapsed_time = float(result.stderr.strip())
    # print(elapsed_time)
    return elapsed_time

def main():
    # Get the maximum number of cores on the machine
    max_cores = os.cpu_count()
    
    n_pods = 2 ** int(math.log2(max_cores))
    
    # Initialize lists to store data for plotting
    cores_list = []
    times_list = []

    # Run the simulation for different numbers of cores
    num_cores = 1
    with tqdm(total=n_pods) as pbar:
        while num_cores <= n_pods:
            elapsed_time = run_simulation(num_cores, n_pods)
            print(f"Number of cores: {num_cores}, Elapsed time: {elapsed_time:.2f} seconds")

            cores_list.append(num_cores)
            times_list.append(elapsed_time)

            # Double the number of cores for the next iteration
            num_cores *= 2
            
            pbar.update(1)

    # Plot the results
    plt.plot(cores_list, times_list, marker='o')
    plt.title('Simulation Time vs Number of Cores')
    plt.xlabel('Number of Cores')
    plt.ylabel('Elapsed Time (seconds)')
    plt.grid(True)
    plt.savefig('simulation_plot.png')
    plt.show()

if __name__ == "__main__":
    main()