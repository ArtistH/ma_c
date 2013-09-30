CC = gcc
AR = ar

SRC = $(wildcard src/*.c)
OBJ = $(addprefix src/,$(notdir $(SRC:.c=.o)))

EXAMPLES = $(wildcard examples/*.c)
EXAMPLES_EXE = $(addprefix examples/,$(notdir $(EXAMPLES:.c=.bin)))

CFLAGS := -I ./include -std=gnu99 -Wall -Werror -Wno-unused -O3 -g -fPIC -lm
LFLAGS := -shared -g -ggdb -lm

DYNAMIC = libbasic.so
STATIC = libbasic.a

.PHONY: all lib clean

all: lib $(EXAMPLES_EXE)

lib: $(DYNAMIC) $(STATIC)

$(DYNAMIC): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@

$(STATIC): $(OBJ)
	$(AR) rcs $@ $(OBJ)

%.o: %.c
	$(CC) $< -c $(CFLAGS) -o $@

$(EXAMPLES_EXE): $(STATIC)

%.bin: %.c
	$(CC) $^ $(CFLAGS) -o $@

clean:
	rm -f $(OBJ) $(STATIC) $(DYNAMIC) $(EXAMPLES_EXE)
