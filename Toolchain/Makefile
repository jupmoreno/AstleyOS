include ../Makevars

DIRS = $(wildcard */.)

all:
	$(foreach DIR, $(DIRS), $(MAKE) -C $(DIR) all;)

clean:
	$(foreach DIR, $(DIRS), $(MAKE) -C $(DIR) clean;)

.PHONY: all clean
