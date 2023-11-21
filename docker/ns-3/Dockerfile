FROM never0lie/scalasca:latest

USER root

RUN apt-get update && \
    apt-get -y install python3 cmake ninja-build git ccache

USER ubuntu
WORKDIR /home/ubuntu

COPY --chown=ubuntu ./ns-allinone-3.40 ns-3

WORKDIR /home/ubuntu/ns-3/ns-3.40
RUN ./ns3 distclean
RUN ./ns3 clean
RUN ./ns3 configure --build-profile=debug --enable-examples --enable-tests --enable-python-bindings --enable-mpi
RUN ./ns3 build -j8