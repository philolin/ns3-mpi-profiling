#!/bin/bash

sudo apt update
sudo apt install -y g++ python3 cmake ninja-build git
sudo apt install -y ccache

# dependency for pyviz (does not work)
# python3 -m pip install --user cppyy
# sudo apt install -y gir1.2-goocanvas-2.0 python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython3  

cd ns-allinone-3.40/ns-3.40
./ns3 clean
./ns3 configure --build-profile=debug --enable-examples --enable-tests --enable-python-bindings
./ns3 build -j8
cd ns-allinone-3.40/ns-3.40/
