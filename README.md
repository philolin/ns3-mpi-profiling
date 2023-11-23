# ns3-optimization

Philo Lin and Sizhan Xu

## Steps to Run
1. `./startup.sh`
2. `./ns3 run scratch/k-fat-tree.cc`
3. `./ns3 run src/mpi/examples/simple-distributed --command-template="mpiexec -np 2 %s"`


## To use scalasca to profile MPI runs
Run `install_scalasca.sh`.

## Generate Graphs
1. `cd plots/`
2. `python3 generate-graphs.py`

Generates graphs for each run and outputs metadata into `outputs/`

3. `python3 average-graphs.py`

Averages the metadata in `outputs/` into one graph

