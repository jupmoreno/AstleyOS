# Scylla
[![Build Status](https://travis-ci.com/jpmrno/Scylla.svg?token=rrzVPFKtpAUUPF2Pp1UE&branch=master)](https://travis-ci.com/jpmrno/Scylla)

> This project was made for our Operating Systems subject at ITBA University (Buenos Aires, Argentina).

Scylla is based on [x64BareBones](https://bitbucket.org/RowDaBoat/x64barebones/) which is a basic setup to develop operating systems for the Intel 64 bits architecture and a provider of an entry point for a C kernel and the possibility to load extra binary modules separated from the main kernel.

It uses a [modification](https://github.com/jpmrno/Scylla/wiki/Pure64:-Modifications) of the [Pure64](https://github.com/ReturnInfinity/Pure64) boot loader to boot up the basic system. The modification enables to load a greater payload than the restrictive 26k that the original allows.

## Requisites

This project needs the following packages: `nasm gcc binutils make qemu`.

### Linux

This will install `make`, `build-essential`, `nasm` and `qemu` into your system.

From the project's directory, run:

	sudo add-apt-repository ppa:ubuntu-toolchain-r/test
	sudo apt-get update
	sudo apt-get install make gcc-5 build-essential nasm qemu

Or simply run the installation script: `./Scripts/install-linux.sh`.

### OS X

*Assuming you already have [homebrew](http://brew.sh) installed...*

This will install `nasm`, `qemu` and a `cross compiler` ([CrossGCC](http://crossgcc.rts-software.org/doku.php?id=compiling_for_linux#bit_linux_binaries_for_mac_intel1)) into your system.

From the project's directory, run:

	brew update
	brew install nasm
	brew install qemu
	curl -o gcc-4.8.1-OSX64.dmg http://crossgcc.rts-software.org/download/gcc-4.8.1-for-linux32-linux64/gcc-4.8.1-for-linux64.dmg
	hdiutil mount gcc-4.8.1-OSX64.dmg
	sudo installer -package /Volumes/gcc-4.8.1-for-linux64/gcc-4.8.1-for-linux64.pkg -target /
	hdiutil detach /Volumes/gcc-4.8.1-for-linux64
	rm -f gcc-4.8.1-OSX64.dmg

Or simply run the installation script: `./Scripts/install-osx.sh`.

## Usage

From the project's directory, simply run:

	make all

If you want to just build the project, run `make build` then use `make run` to run.

## Credits

- Scylla β (project's structure & kernel modifications):
	- Author: Juan Moreno

- Scylla α (kernel implementation):
	- Author: [Matías Comercio](https://github.com/MatiasComercioITBA)
	- Author: Juan Moreno

- x64BareBones (base project):
	- Author: Rodrigo Rearden
	- Collaborator: Augusto Nizzo McIntosh
