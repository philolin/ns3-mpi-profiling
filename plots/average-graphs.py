import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import json

# Function to read a file and extract the y list
def read_file(file_path):
    with open(file_path) as file:
        x = json.loads(file.readline())
        y = json.loads(file.readline())
    return x, y

# # Function to average a list of y lists
def average_y_lists(y_lists):
    return np.mean(y_lists, axis=0)

# List of file paths
path = "outputs/"
file_paths = [f'{path}run-0-output.txt', f'{path}run-1-output.txt', f'{path}run-2-output.txt', f'{path}run-3-output.txt', f'{path}run-4-output.txt']

# Read y lists from each file
y_lists = [read_file(file_path)[1] for file_path in file_paths]

# Average y lists
averaged_y = average_y_lists(y_lists)

x = read_file(file_paths[0])[0]

# Plot the result
matplotlib.use('Agg')  # Use Agg backend (for PNGs) that doesn't require a display
plt.plot(x, averaged_y)
plt.grid(True)
plt.xlabel('Cores')
plt.ylabel('Wallclock Time (seconds)')
plt.title('Average Wallclock Time over 5 runs')
plt.savefig(f'averaged-simulation-plot.png')
# plt.show()

  
# Log output
with open(f'outputs/averaged-output.txt', "w") as f:
    f.write(f'{x}\n{averaged_y.tolist()}')
