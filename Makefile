include Makevars

help:
	@echo "Alma's makefile"
	@echo "Usage: 'make [ all | reset | clean | build | run | help ]'"

all: clean build run

build: toolchain bootloader libs kernel userland image

toolchain:
	$(MAKE) -C $(TOOLCHAIN_PATH) all

bootloader:
	$(MAKE) -C $(BOOTLOADER_PATH) all

libs:
	$(MAKE) -C $(LIBS_PATH) all

kernel: libs
	$(MAKE) -C $(KERNEL_PATH) all

userland: libs
	$(MAKE) -C $(USERLAND_PATH) all

image: toolchain bootloader libs kernel userland
	$(MAKE) -C $(IMAGE_PATH) all

clean:
	$(MAKE) -C $(LIBS_PATH) clean
	$(MAKE) -C $(KERNEL_PATH) clean
	$(MAKE) -C $(USERLAND_PATH) clean
	$(MAKE) -C $(IMAGE_PATH) clean

reset: clean
	$(MAKE) -C $(TOOLCHAIN_PATH) clean
	$(MAKE) -C $(BOOTLOADER_PATH) clean

run: image
	$(QEMU) $(QEMU_FLAGS) -hda $(SOURCE_IMAGE) $(QEMU_SOUND)

.PHONY: help all build toolchain bootloader libs kernel userland image clean reset run
