#!/usr/bin/python3
import subprocess
import matplotlib.pyplot as plt
import os
from pprint import pprint
import numpy as np
import matplotlib
matplotlib.use('Agg')

def parse_scorep(scorep_path):
    # sed -n '/MPI_/p' scorep.score 
    command = ['sed','-n','/MPI_/p',scorep_path]
    # Run the command
    result = subprocess.run(command, text=True, capture_output=True)
    # print(result.stdout)
    return result.stdout

def main():
    base_folder_path = 'scalasca'

    # List all files in the folder
    folders = os.listdir(base_folder_path)

    # Iterate through each file
    for folder_name in folders:
        folder_path = os.path.join(base_folder_path, folder_name)
        subfolders = os.listdir(folder_path)

        mpi_function_set = set()
        for subfolder_name in subfolders:
            subfolder_path = os.path.join(folder_path, subfolder_name)
            scorep_path = subfolder_path + "/scorep.score"
            result = parse_scorep(scorep_path)

            # Split the input data into lines
            lines = result.strip().split('\n')

            # Extract the last element from each line and put them into a list
            current_mpi_functions = set(line.split()[-1] for line in lines)
            mpi_function_set |= current_mpi_functions
        # print(mpi_function_set)

        mpi_functions = {}
        for subfolder_name in subfolders:
            subfolder_path = os.path.join(folder_path, subfolder_name)
            scorep_path = subfolder_path + "/scorep.score"
            result = parse_scorep(scorep_path)

            # Split the input data into lines
            lines = result.strip().split('\n')

            # Extract the last element from each line and put them into a list
            current_mpi_functions = [line.split()[-1] for line in lines]
            current_mpi_times = [line.split()[3] for line in lines]

            for function in mpi_function_set:
                if function not in current_mpi_functions:
                    time = 0.00
                else:
                    index = current_mpi_functions.index(function)
                    time = current_mpi_times[index]
            
                if function not in mpi_functions:
                        mpi_functions[function] = [time]
                else:
                    mpi_functions[function].append(time)

        # Iterate through the dictionary and remove items with all zero values
        keys_to_remove = [key for key, value in mpi_functions.items() if all(val == '0.00' for val in value)]
        for key in keys_to_remove:
            del mpi_functions[key]

        # Extract data for plotting
        categories = list(mpi_functions.keys())
        values = np.array([list(map(float, entry)) for entry in mpi_functions.values()])

        # Number of bars per group
        num_bars = len(mpi_functions[next(iter(mpi_functions))])

        # Width of each bar
        bar_width = 0.2

        # Set up positions for bars in each group
        positions = np.arange(len(categories))

        # Create a grouped bar chart
        for i in range(num_bars):
            plt.bar(positions + i * bar_width, values[:, i], width=bar_width, label=f'{2**i} Cores')

        # Set y-axis to logarithmic scale and adjust the limit
        plt.yscale('log')
        # plt.ylim(0.000, max(np.max(values), 1))  # Adjust the lower limit as needed
        
        plt.xlabel('MPI Function Name')
        plt.ylabel('Time (Seconds)')
        plt.title(f'{folder_name} Grouped Bar Chart')
        plt.xticks(positions + bar_width * (num_bars - 1) / 2, categories, rotation=45, ha='right')
        plt.legend(title='Bars')
        plt.tight_layout()

        plt.savefig(f'plots/{folder_name}-stacked-bar-plot.png')
        plt.clf()
        # plt.show()

if __name__ == "__main__":
    main()