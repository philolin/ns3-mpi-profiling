scalasca_fat_tree:
	cd ns-allinone-3.40/ns-3.40/cmake-cache && \
	/usr/bin/ccache scorep /usr/bin/c++ -DHAVE_PACKET_H -DNS3_ASSERT_ENABLE -DNS3_BUILD_PROFILE_DEBUG -DNS3_LOG_ENABLE -DNS3_MPI -DPROJECT_SOURCE_PATH=\"/users/sizhanx/ns3-optimization/ns-allinone-3.40/ns-3.40\" -DRAW_SOCK_CREATOR=\"/users/sizhanx/ns3-optimization/ns-allinone-3.40/ns-3.40/build/src/fd-net-device/ns3.40-raw-sock-creator-debug\" -DTAP_DEV_CREATOR=\"/users/sizhanx/ns3-optimization/ns-allinone-3.40/ns-3.40/build/src/fd-net-device/ns3.40-tap-device-creator-debug\" -D__LINUX__ -I/users/sizhanx/ns3-optimization/ns-allinone-3.40/ns-3.40/build/include -I/usr/include/python3.10 -I/usr/lib/x86_64-linux-gnu/openmpi/include -I/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi -g -fPIE   -fno-semantic-interposition -fdiagnostics-color=always -Wall -Werror -Wno-error=deprecated-declarations -std=c++17 -Winvalid-pch -include /users/sizhanx/ns3-optimization/ns-allinone-3.40/ns-3.40/cmake-cache/CMakeFiles/stdlib_pch_exec.dir/cmake_pch.hxx -MD -MT scratch/CMakeFiles/scratch_fat-tree-animation-mpi.dir/fat-tree-animation-mpi.cc.o -MF scratch/CMakeFiles/scratch_fat-tree-animation-mpi.dir/fat-tree-animation-mpi.cc.o.d -o scratch/CMakeFiles/scratch_fat-tree-animation-mpi.dir/fat-tree-animation-mpi.cc.o -c /users/sizhanx/ns3-optimization/ns-allinone-3.40/ns-3.40/scratch/fat-tree-animation-mpi.cc && \
	scorep /usr/bin/c++ -g  scratch/CMakeFiles/scratch_fat-tree-animation-mpi.dir/fat-tree-animation-mpi.cc.o -o ../build/scratch/ns3.40-fat-tree-animation-mpi-debug -L/users/sizhanx/ns3-optimization/ns-allinone-3.40/ns-3.40/build/lib -Wl,-rpath,/users/sizhanx/ns3-optimization/ns-allinone-3.40/ns-3.40/build/lib:/usr/lib/x86_64-linux-gnu/openmpi/lib  -Wl,--no-as-needed  ../build/lib/libns3.40-topology-read-debug.so  ../build/lib/libns3.40-tap-bridge-debug.so  ../build/lib/libns3.40-sixlowpan-debug.so  ../build/lib/libns3.40-olsr-debug.so  ../build/lib/libns3.40-nix-vector-routing-debug.so  ../build/lib/libns3.40-netanim-debug.so  ../build/lib/libns3.40-mesh-debug.so  ../build/lib/libns3.40-lte-debug.so  ../build/lib/libns3.40-lr-wpan-debug.so  ../build/lib/libns3.40-internet-apps-debug.so  ../build/lib/libns3.40-flow-monitor-debug.so  ../build/lib/libns3.40-fd-net-device-debug.so  ../build/lib/libns3.40-dsr-debug.so  ../build/lib/libns3.40-dsdv-debug.so  ../build/lib/libns3.40-csma-layout-debug.so  ../build/lib/libns3.40-csma-debug.so  ../build/lib/libns3.40-config-store-debug.so  ../build/lib/libns3.40-buildings-debug.so  ../build/lib/libns3.40-applications-debug.so  ../build/lib/libns3.40-aodv-debug.so  -Wl,--as-needed  ../build/lib/libns3.40-wimax-debug.so  ../build/lib/libns3.40-uan-debug.so  ../build/lib/libns3.40-point-to-point-layout-debug.so  ../build/lib/libns3.40-virtual-net-device-debug.so  ../build/lib/libns3.40-point-to-point-debug.so  ../build/lib/libns3.40-mpi-debug.so  /usr/lib/x86_64-linux-gnu/openmpi/lib/libmpi_cxx.so  /usr/lib/x86_64-linux-gnu/openmpi/lib/libmpi.so  ../build/lib/libns3.40-wifi-debug.so  ../build/lib/libns3.40-spectrum-debug.so  ../build/lib/libns3.40-propagation-debug.so  ../build/lib/libns3.40-mobility-debug.so  ../build/lib/libns3.40-energy-debug.so  ../build/lib/libns3.40-antenna-debug.so  ../build/lib/libns3.40-internet-debug.so  ../build/lib/libns3.40-traffic-control-debug.so  ../build/lib/libns3.40-bridge-debug.so  ../build/lib/libns3.40-network-debug.so  ../build/lib/libns3.40-stats-debug.so  ../build/lib/libns3.40-core-debug.so  -Wl,--no-as-needed  -Wl,--as-needed && \
	cd -