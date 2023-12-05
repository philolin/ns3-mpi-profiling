import matplotlib.pyplot as plt

def main():
  core_num = [0,1,2,3,4,5,6,7]
  computation_time = [84.07, 70.90, 70.44, 69.46, 70.76, 69.16, 69.57, 69.56]
  mpi_time = [4.47, 17.60, 18.03, 19.03, 17.17, 19.31,18.88, 18.96]
  
  zipped = list(zip(core_num, computation_time, mpi_time))
  
  zipped = sorted(zipped, key=lambda x: x[1])
  
  core_num_sorted, computation_time_sorted, mpi_time_sorted = zip(*zipped)
  
  print(core_num_sorted)
  print(computation_time_sorted)
  print(mpi_time_sorted)
  
  fig, ax = plt.subplots()
  
  ax.bar(core_num_sorted, computation_time_sorted, label="Computation Time")
  ax.bar(core_num_sorted, mpi_time_sorted, bottom=computation_time_sorted, label="MPI Time")

  ax.set_xlabel("Rank Index")
  ax.set_ylabel("Time(s)")
  
  ax.set_title("Computation and MPI Time Taken on Ranks")
  
  ax.legend()
  
  plt.show()
  

if __name__ == "__main__":
  main()
