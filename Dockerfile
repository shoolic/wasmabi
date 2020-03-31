FROM ubuntu:18.04 as builder
RUN apt-get update \
    && apt-get -y install \
    cmake \
    ninja-build \
    gdb

WORKDIR /wasmabi

COPY create_configs.sh CMakeLists.txt ./
COPY include include
COPY src src

RUN ./create_configs.sh \
    && cd build/Release \
    && make

FROM ubuntu:18.04
WORKDIR /wasmabi
COPY --from=builder /wasmabi/bin/Release/wasmabi .
ENTRYPOINT ./wasmabi