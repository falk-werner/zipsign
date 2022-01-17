FROM centos:centos7 as builder

RUN set -x \
    && yum -y update \
    && yum -y upgrade \
    && yum -y group install 'Development Tools' \
    && yum -y install \
        pkg-config \
        curl \
        ca-certificates \
        zlib-devel \
        which

ARG BUILD_DIR=/tmp/build

ARG OPENSSL_VERSION=1.1.1m
RUN set -x \
    && mkdir -p "${BUILD_DIR}" \
    && cd "${BUILD_DIR}" \
    && curl -fSL -s -o openssl.tar.gz https://ftp.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz \
    && tar -xf openssl.tar.gz \
    && cd openssl-${OPENSSL_VERSION} \
    && ./config --prefix=/usr --openssldir=/etc/ssl zlib-dynamic \
    && make \
    && make install \
    && cd \
    && rm -rf "${BUILD_DIR}"

ARG CMAKE_VERSION=3.22.1
RUN set -x \
    && mkdir -p "${BUILD_DIR}" \
    && cd "${BUILD_DIR}" \
    && curl -fSL -s -o cmake.tar.gz https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}.tar.gz \
    && tar -xf cmake.tar.gz \
    && cd cmake-${CMAKE_VERSION} \
    && ./bootstrap \
    && make \
    && make install \
    && cd \
    && rm -rf "${BUILD_DIR}"

ARG GTEST_VERSION=1.10.0
RUN set -x \
  && mkdir -p "${BUILD_DIR}" \
  && cd "${BUILD_DIR}" \
  && curl -fSL -s -o gtest.tar.gz https://github.com/google/googletest/archive/release-${GTEST_VERSION}.tar.gz \
  && tar -xf gtest.tar.gz \
  && cd googletest-release-${GTEST_VERSION} \
  && mkdir build \
  && cd build \
  && cmake -DCMAKE_INSTALL_PREFIX=/usr .. \
  && make \
  && make install \
  && cd \
  && rm -rf "${BUILD_DIR}"

ARG USER_ID=1000
RUN set -x \
    && useradd -u "$USER_ID" -ms /bin/bash user \
    && true

ARG SOURCE_DIR=/home/user/zipsign
COPY . ${SOURCE_DIR}

RUN set -x \
    && mkdir -p "${SOURCE_DIR}/build" \
    && cd "${SOURCE_DIR}/build" \
    && cmake .. \
    && make

RUN set -x \
  && chown -R user:user ${SOURCE_DIR}

WORKDIR ${SOURCE_DIR}
ENTRYPOINT ["bash"]