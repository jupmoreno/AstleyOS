#!/bin/bash
set -ev
brew update
brew install nasm
brew install qemu
curl -o gcc-4.8.1-OSX64.dmg http://crossgcc.rts-software.org/download/gcc-4.8.1-for-linux32-linux64/gcc-4.8.1-for-linux64.dmg
hdiutil mount gcc-4.8.1-OSX64.dmg
sudo installer -package /Volumes/gcc-4.8.1-for-linux64/gcc-4.8.1-for-linux64.pkg -target /
hdiutil detach /Volumes/gcc-4.8.1-for-linux64
rm -f gcc-4.8.1-OSX64.dmg
