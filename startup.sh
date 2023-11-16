#!/bin/bash

sudo apt update
sudo apt install -y g++ python3 cmake ninja-build git
sudo apt install -y ccache

# netanim
sudo apt install -y qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools

# mpi
sudo apt install -y openmpi-bin openmpi-common openmpi-doc libopenmpi-dev
export PATH=$PATH:/usr/lib64/openmpi/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/openmpi/lib

cd ns-allinone-3.40/ns-3.40
./ns3 distclean
./ns3 clean
./ns3 configure --build-profile=debug --enable-examples --enable-tests --enable-python-bindings --enable-mpi
./ns3 build -j8

# graph
sudo apt install -y python3-pip
pip install -r $(pwd)/requirements.txt
