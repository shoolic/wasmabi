FROM ubuntu:18.04 as builder
RUN apt-get update \
    && apt-get -y install \
    cmake \
    ninja-build \
    gdb 

WORKDIR /wasmabi
COPY .vscode .vscode
COPY create_configs.sh CMakeLists.txt ./
COPY include include
COPY src src
RUN ./create_configs.sh \
    && cd build/Debug \
    && make