FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++ \
    gcc \
    gdb \
    cmake \
    make \
    ninja-build \
    curl \
    libcurl4-openssl-dev \
    libjsoncpp-dev \
    git \
    unzip \
    wget \
    pkg-config \
    build-essential \
    astyle \
    libgtest-dev

WORKDIR /fakerest-test

CMD rm -rf build && mkdir -p build && cd build && cmake .. && make && ./fakerest
