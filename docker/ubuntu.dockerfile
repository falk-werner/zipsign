ARG REGISTRY_PREFIX=''
ARG CODENAME=focal

FROM ${REGISTRY_PREFIX}ubuntu:${CODENAME} as builder

ARG DEBIAN_FRONTEND=noninteractive
RUN set -x \
    && apt update \
    && apt upgrade -y \
    && apt install --yes --no-install-recommends \
        build-essential \
        pkg-config \
        cmake \
        curl \
        ca-certificates \
        openssl \
        libssl-dev

ARG BUILD_DIR=/tmp/build
ARG DUMB_INIT_VERSION=1.2.2
RUN set -x \
  && mkdir -p "${BUILD_DIR}" \
  && cd "${BUILD_DIR}" \
  && curl -fSL -s -o dumb-init-${DUMB_INIT_VERSION}.tar.gz https://github.com/Yelp/dumb-init/archive/v${DUMB_INIT_VERSION}.tar.gz \
  && tar -xf dumb-init-${DUMB_INIT_VERSION}.tar.gz \
  && cd "dumb-init-${DUMB_INIT_VERSION}" \
  && make \
  && chmod +x dumb-init \
  && mv dumb-init /usr/local/bin/dumb-init \
  && dumb-init --version \
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
  && cmake .. \
  && make \
  && make install \
  && cd \
  && rm -rf "${BUILD_DIR}"

ARG USERID=1000
RUN set -x \
    && useradd -u "$USERID" -ms /bin/bash user

ARG SOURCE_DIR=/home/user/zipsign
COPY . ${SOURCE_DIR}

RUN set -x \
    && mkdir -p "${SOURCE_DIR}/build" \
    && cd "${SOURCE_DIR}/build" \
    && cmake .. \
    && make \
    && ./alltests

RUN set -x \
  && chown -R user:user ${SOURCE_DIR}

WORKDIR ${SOURCE_DIR}
ENTRYPOINT ["dumb-init", "--", "bash"]