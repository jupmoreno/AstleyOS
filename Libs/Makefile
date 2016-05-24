include ../Makevars
include Makevars

LIB = lib$(LIB_NAME).a

SOURCES_C = $(wildcard $(SRC_C)/*.c & $(SRC_C)/**/*.c)
SOURCES_ASM = $(wildcard $(SRC_ASM)/*.asm & $(SRC_ASM)/**/*.asm)

OBJECTS_C = $(SOURCES_C:.c=.o)
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)

HEADERS = $(foreach HEADER,$(SRC_HEADERS),-I $(HEADER))

all: $(LIB)

$(LIB): $(OBJECTS_C) $(OBJECTS_ASM)
	$(AR) $(AR_FLAGS) $@ $(OBJECTS_C) $(OBJECTS_ASM)

%.o: %.c
	$(GCC) $(GCC_FLAGS) $(HEADERS) -c $< -o $@

%.o: %.asm
	$(ASM) $(ASM_FLAGS) $< -o $@

clean:
	rm -f $(LIB) $(OBJECTS_C) $(OBJECTS_ASM)

.PHONY: all clean
