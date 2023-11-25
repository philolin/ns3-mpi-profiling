#!/bin/bash

# Check if the script is run as root
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root or with sudo."
   exit 1
fi

apt update && \
apt install binutils-dev

cd /opt
wget https://adv-network.us-southeast-1.linodeobjects.com/scalasca.zip
unzip scalasca.zip

export PATH="/opt/scorep/bin:$PATH"
export PATH="/opt/cubelib/bin:$PATH"
export PATH="/opt/scalasca/bin:$PATH"

rm scalasca.zip

cd -

cat << 'EOF'
Scalasca environment has been set up in this terminal session.
To set it permanently, please put this into your ~/.bashrc file

export PATH="/opt/scorep/bin:$PATH"
export PATH="/opt/cubelib/bin:$PATH"
export PATH="/opt/scalasca/bin:$PATH"
EOF
