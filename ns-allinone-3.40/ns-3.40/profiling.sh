#!/bin/bash
mkdir profiling

# ==29415== 
# ==29415== HEAP SUMMARY:
# ==29415==     in use at exit: 0 bytes in 0 blocks
# ==29415==   total heap usage: 238,838 allocs, 238,838 frees, 11,343,285 bytes allocated
# ==29415== 
# ==29415== All heap blocks were freed -- no leaks are possible
# ==29415== 
# ==29415== For lists of detected and suppressed errors, rerun with: -s
# ==29415== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
./ns3 run scratch/fat-tree-animation.cc --valgrind 2> profiling/valgrind.txt

# heaptrack stats:
#         allocations:            238838
#         leaked allocations:     27725
#         temporary allocations:  37413
# Heaptrack finished! Now run the following to investigate the data:
rm heaptrack.ns3.40-fat-tree-animation-debug.*.zst
./ns3 run scratch/fat-tree-animation.cc --heaptrack
heaptrack --analyze "/users/philolin/ns3-optimization/ns-allinone-3.40/ns-3.40/heaptrack.ns3.40-fat-tree-animation-debug.*.zst" 2> profiling/heaptrack.txt

# strace
# % time     seconds  usecs/call     calls    errors syscall
# ------ ----------- ----------- --------- --------- ----------------
#  51.92    0.000691           3       207           mmap
#  15.40    0.000205           3        60        17 openat
#   9.24    0.000123           2        53         9 newfstatat
#   8.79    0.000117           2        41           read
#   8.19    0.000109           1        64           mprotect
#   5.48    0.000073           1        43           close
#   0.53    0.000007           1         4           pread64
#   0.15    0.000002           2         1           set_tid_address
#   0.15    0.000002           2         1           set_robust_list
#   0.08    0.000001           0         2         1 arch_prctl
#   0.08    0.000001           1         1           rseq
#   0.00    0.000000           0        86           write
#   0.00    0.000000           0         2           munmap
#   0.00    0.000000           0        26           brk
#   0.00    0.000000           0         1         1 access
#   0.00    0.000000           0         1           execve
#   0.00    0.000000           0         1           futex
#   0.00    0.000000           0         1           prlimit64
#   0.00    0.000000           0         1           getrandom
# ------ ----------- ----------- --------- --------- ----------------
# 100.00    0.001331           2       596        28 total
./ns3 run scratch/fat-tree-animation.cc --command-template "strace -c %s" 2> profiling/strace.txt