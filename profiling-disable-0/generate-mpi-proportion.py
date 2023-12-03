import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import PercentFormatter
import subprocess

def get_mpi_percentage(filename):
  sed_output = subprocess.run(["sed", "-n", '10,14p', filename], stdout=subprocess.PIPE, text=True).stdout

  lines = [x.strip() for x in sed_output.splitlines()]
  calculation_time = 0;
  mpi_time = 0;
  for line in lines:
    words = list(filter(lambda word: len(word) > 0, line.split(" ")))
    if words[0] == "USE" or words[0] == "COM":
      calculation_time += float(words[3])
    elif words[0] == "MPI":
      mpi_time += float(words[3])
  
  return mpi_time / (calculation_time + mpi_time)

def path_builder(run_idx, core_num):
  return f"scalasca/run-{run_idx}/scorep_ns3_{core_num}_sum/scorep.score"


def main():
  num_runs = 3
  cores = [1,2,4,8]
  
  raw_data: list[list[int]] = []
  for i, v in enumerate(cores):
    raw_data.append([])
    for j in range(num_runs):
      raw_data[i].append(get_mpi_percentage(path_builder(j, v)))
      
  avg_data: list[int] = []
  
  for ptgs in raw_data:
    sum = 0
    for ptg in ptgs:
      sum += ptg
    sum /= num_runs
    avg_data.append(sum)
  
  print(avg_data)
  
  x_labels = [f"{core} core{'s' if core > 1 else ''}\n MPI: {avg_data[idx]:.3%}" for idx, core in enumerate(cores)]

  # Preparing y values for the stacked bar chart
  y_values = {
      "Below": [1 - p for p in avg_data],
      "Above": [p for p in avg_data],
  }
    
  # Plotting the stacked bar chart
  fig, ax = plt.subplots()
  ax.bar(x_labels, y_values['Below'], label='Computation')
  ax.bar(x_labels, y_values['Above'], bottom=y_values['Below'], label='MPI')

  # Adding labels and title
  ax.set_title('MPI time percentage for parallel NS-3 using different number of cores')
  ax.set_ylabel('Percentage')
  ax.set_xlabel('Number of Cores')
  ax.yaxis.set_major_formatter(PercentFormatter(1))
  ax.legend(loc="best")

  plt.show()
    

if __name__ == "__main__":
  main()