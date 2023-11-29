#!/usr/bin/python3
import subprocess
import matplotlib.pyplot as plt
import os
import math

def run_simulation(num_cores, n_pods, run):
    command = f'mkdir -p scalasca/run-{run} && cd scalasca/run-{run}; /usr/bin/time -f "%e" scan mpirun --oversubscribe -np {num_cores} ../../../ns-allinone-3.40/ns-3.40/build/scratch/ns3.40-fat-tree-animation-mpi-debug --nPods={n_pods}'
    print(command)
    result = subprocess.run(command, shell=True, stderr=subprocess.PIPE, universal_newlines=True)
    print(result.stderr)
    # Extract the real time from the output
    elapsed_time = float(result.stderr.splitlines()[-1])
    return elapsed_time

def log():
    RUNS = 3
    
    # Get the maximum number of cores on the machine
    max_cores = os.cpu_count()
    n_pods = 2 ** int(math.log2(max_cores))
    n_pods = 8
    
    for i in range(RUNS):
        # Initialize lists to store data for plotting
        cores_list = []
        times_list = []
        print(f"On run {i} ...")   
        # Run the simulation for different numbers of cores
        num_cores = n_pods
        while num_cores > 0:
            elapsed_time = run_simulation(num_cores, n_pods, i)
            print(f"Number of cores: {num_cores}, Elapsed time: {elapsed_time} seconds")    
                
            cores_list.append(num_cores)
            times_list.append(elapsed_time)

            # Double the number of cores for the next iteration
            num_cores //= 2
        
        # Plot the results
        plt.plot(cores_list, times_list, marker='o')
        plt.title('Simulation Time vs Number of Cores')
        plt.xlabel('Number of Cores')
        plt.ylabel('Elapsed Time (seconds)')
        plt.grid(True)
        plt.savefig(f'plots/{n_pods}-cores-run-{i}-simulation_plot.png')
        # plt.show()
        
        # Log output
        with open(f'outputs/run-{i}-output.txt', "w") as f:
            f.write(f'{cores_list}\n{times_list}')
    
def examine():
    base_folder_path = 'scalasca'

    # List all files in the folder
    folders = os.listdir(base_folder_path)

    # Iterate through each file
    for folder_name in folders:
        folder_path = os.path.join(base_folder_path, folder_name)
        subfolders = os.listdir(folder_path)
        for subfolder_name in subfolders:
            subfolder_path = os.path.join(folder_path, subfolder_name)
            print(subfolder_path)
            command = ['scalasca','-examine','-s',subfolder_path]
            # Run the command
            subprocess.run(command)


def main():
    log()
    examine()

if __name__ == "__main__":
    main()
