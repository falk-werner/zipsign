FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && apt install -y --no-install-recommends \
    locales \
    ca-certificates \
    git \
    openssh-client \
    build-essential \
    pkg-config \
    cmake \
    libgtest-dev \
    libgmock-dev \
    clang-tidy \
    valgrind \
    lcov \
    libssl-dev \
    openssl

RUN sed -i '/en_US.UTF-8/s/^# //g' /etc/locale.gen && locale-gen
ENV LANG en_US.UTF-8  
ENV LANGUAGE en_US:en  
ENV LC_ALL en_US.UTF-8

ARG USERNAME=user
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME
RUN useradd --uid $USER_UID --gid $USER_GID -m $USERNAME
