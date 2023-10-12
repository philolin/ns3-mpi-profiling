#!/bin/bash

sudo apt-get update
sudo apt-get -y install cmake

cd ns-allinone-3.40/ns-3.40
./ns3 clean
./ns3 configure --build-profile=debug --enable-examples --enable-tests
./ns3 build -j8
