#!/bin/bash
set -ev
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update -qq
sudo apt-get install make gcc-5 build-essential nasm qemu -y
