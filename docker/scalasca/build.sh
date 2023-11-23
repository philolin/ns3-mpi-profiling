#!/bin/bash
wget https://perftools.pages.jsc.fz-juelich.de/cicd/scorep/tags/scorep-8.3/scorep-8.3.tar.gz -O scorep.tar.gz
wget https://apps.fz-juelich.de/scalasca/releases/cube/4.8/dist/cubelib-4.8.2.tar.gz -O cubelib.tar.gz
wget https://apps.fz-juelich.de/scalasca/releases/scalasca/2.6/dist/scalasca-2.6.1.tar.gz -O scalasca.tar.gz

tar -zxvf scorep.tar.gz
tar -zxvf cubelib.tar.gz
tar -zxvf scalasca.tar.gz

rm *.tar.gz

docker build . -t never0lie/scalasca && \
docker push never0lie/scalasca

rm -rf scorep*
rm -rf cubelib*
rm -rf scalasca*