# syntax=docker.io/docker/dockerfile:1
FROM --platform=linux/riscv64 cartesi/node:20.8.0-jammy-slim

ARG LIBCMT_VERSION=0.15.0
ADD https://github.com/cartesi/machine-emulator-tools/releases/download/v${LIBCMT_VERSION}/libcmt-v${LIBCMT_VERSION}-dev.deb /
RUN dpkg -i /libcmt-v${LIBCMT_VERSION}-dev.deb \
  && rm /libcmt-v${LIBCMT_VERSION}-dev.deb

ARG DEBIAN_FRONTEND=noninteractive
RUN <<EOF
set -e
apt-get update
apt-get install -y --no-install-recommends \
  python3-minimal=3.10.6-1~22.04
rm -rf /var/lib/apt/lists/* /var/log/* /var/cache/*
corepack enable pnpm
EOF

WORKDIR /usr/src
COPY . .
RUN pnpm install
