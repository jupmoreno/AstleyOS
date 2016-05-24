include ../Makevars
include Makevars

USERLAND_FILES = $(wildcard ../$(USERLAND_PATH)/*.bin)

QEMU_CONVERT = qemu-img convert -f raw

all: $(SYSTEM_PACKED) $(IMG) $(VMDK) $(QCOW2)

$(SYSTEM_PACKED): $(USERLAND_FILES)
	../$(MODULE_PACKER) -o $@ ../$(KERNEL) $<

$(IMG): $(SYSTEM_PACKED)
	../$(FILESYSTEM) $(IMG) initialize $(IMG_SIZE) ../$(FILESYSTEM_MBR) ../$(BOOTLOADER) $<

$(VMDK): $(IMG)
	$(QEMU_CONVERT) -O vmdk $(IMG) $(VMDK)

$(QCOW2): $(IMG)
	$(QEMU_CONVERT) -O qcow2 $(IMG) $(QCOW2)

clean:
	rm -f $(SYSTEM_PACKED) $(IMG) $(VMDK) $(QCOW2)

.PHONY: all clean
