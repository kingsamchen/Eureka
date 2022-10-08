FROM ubuntu:22.04

LABEL maintainer="kingsamchen@gmail.com"
LABEL release-date="2022-10-08"

ENV USER=dev
ENV PASSWD=dev

COPY ./sources.list /etc/apt/sources.list

RUN apt-get update && apt-get install -y \
        build-essential \
        cmake clang clangd \
        git gdb \
        net-tools ninja-build \
        python3 python3-pip \
        sudo \
        vim \
    && apt-get clean

RUN update-alternatives --install /usr/bin/cc cc /usr/bin/clang 40 && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++ 40

RUN useradd -s /bin/bash -m ${USER} && yes ${PASSWD} | passwd ${USER} && \
    echo ${USER}' ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers && \
    chmod 644 /etc/sudoers

USER ${USER}
