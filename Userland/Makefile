include ../Makevars

MODULES = $(wildcard */.)

all:
	$(foreach MODULE, $(MODULES), $(MAKE) -C $(MODULE) all;)

clean:
	$(foreach MODULE, $(MODULES), $(MAKE) -C $(MODULE) clean;)

.PHONY: all clean
