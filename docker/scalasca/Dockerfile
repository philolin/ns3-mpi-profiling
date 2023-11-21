FROM ubuntu:jammy

RUN apt-get update && \
  apt-get -y install sudo openmpi-bin openmpi-common openmpi-doc libopenmpi-dev g++ binutils-dev build-essential wget curl

RUN useradd -G sudo -ms /bin/bash ubuntu -p "$(openssl passwd -6 ubuntu)"

COPY --chown=root ./scorep-* /root/scorep
COPY --chown=root ./cubelib-* /root/cubelib
COPY --chown=root ./scalasca-* /root/scalasca

# building scorep
WORKDIR /root/scorep
RUN mkdir _build
WORKDIR /root/scorep/_build

RUN ../configure --with-mpi=openmpi3  --with-shmem=openmpi3 --prefix=/opt/scorep
RUN make -j $(nproc)
RUN make install

ENV PATH="${PATH}:/opt/scorep/bin"

# building cubelib
WORKDIR /root/cubelib
RUN ./configure --with-mpi=openmpi3 --prefix=/opt/cubelib
RUN make -j $(nproc)
RUN make install

ENV PATH="${PATH}:/opt/cubelib/bin"

# building scalasca
WORKDIR /root/scalasca
RUN ./configure --with-mpi=openmpi3 --prefix=/opt/scalasca
RUN make -j $(nproc)
RUN make install

ENV PATH="${PATH}:/opt/scalasca/bin"

USER ubuntu
WORKDIR /home/ubuntu

RUN echo 'export PATH="/opt/scorep/bin:$PATH"' >> ~/.bashrc && \
    echo 'export PATH="/opt/cubelib/bin:$PATH"' >> ~/.bashrc && \
    echo 'export PATH="/opt/scalasca/bin:$PATH"' >> ~/.bashrc