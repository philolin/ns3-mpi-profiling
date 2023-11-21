#!/bin/bash

wget https://www.nsnam.org/releases/ns-allinone-3.40.tar.bz2 -O ns-3.tar.bz2
bzip2 -dc ns-3.tar.bz2 | tar xvf -

rm ns-3.tar.bz2

docker build . -t never0lie/ns-3 && \
docker push never0lie/ns-3

rm -rf ns*