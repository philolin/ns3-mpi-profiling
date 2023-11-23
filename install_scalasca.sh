#!/bin/bash

# Check if the script is run as root
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root or with sudo."
   exit 1
fi

cd /opt
wget https://adv-network.us-southeast-1.linodeobjects.com/scalasca.zip
unzip sclasca.zip

export PATH="/opt/scorep/bin:$PATH"
export PATH="/opt/cubelib/bin:$PATH"
export PATH="/opt/scalasca/bin:$PATH"

cd -