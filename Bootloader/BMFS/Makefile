include ../../Makevars
include Makevars

FILE = ../../$(FILESYSTEM)

SOURCES_C = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)

HEADERS = $(foreach HEADER,$(SRC_HEADERS),-I $(HEADER))

all: $(FILE)

$(FILE): $(SOURCES_C)
	$(SYS_GCC) $(GCC_FLAGS) $(HEADERS) $< -o $@

clean:
	rm -f $(FILE)

.PHONY: all clean
