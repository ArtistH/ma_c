CC = gcc
AR = ar

SRC = $(wildcard src/*.c)
OBJ = $(addprefix src/,$(notdir $(SRC:.c=.o)))

TESTS = $(wildcard tests/*.c)
TESTS_OBJ = $(addprefix tests/,$(notdir $(TESTS :.c=.o)))
TESTS_EXE = ./test ./test.txt

EXAMPLES = $(wildcard examples/*.c)
EXAMPLES_EXE = $(addprefix examples/,$(notdir $(EXAMPLES:.c=.bin)))

CFLAGS := -I ./include -std=gnu99 -Wall -Wextra -Werror -Wno-unused -O3 -g -fPIC
LIBS := -lm
LFLAGS := -shared -g -ggdb

EXE_NAME = Cbasic
DYNAMIC = lib$(EXE_NAME).so
STATIC = lib$(EXE_NAME).a

.PHONY: all lib clean

all: lib $(EXAMPLES_EXE)

lib: $(DYNAMIC) $(STATIC)

$(DYNAMIC): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) $(LIBS) -o $@

$(STATIC): $(OBJ)
	$(AR) rcs $@ $(OBJ)

tests: $(TESTS_OBJ) $(STATIC)
	$(CC) $< $(LIBS) -o test
	./test

%.o: %.c
	$(CC) $< -c $(CFLAGS) -o $@

$(EXAMPLES_EXE): $(STATIC)

%.bin: %.c
	$(CC) $^ $(CFLAGS) $(LIBS) -o $@

clean:
	rm -f $(STATIC) $(DYNAMIC)
	rm -f $(OBJ) $(TESTS_OBJ)
	rm -f $(EXAMPLES_EXE)
	-rm -f $(TESTS_EXE)
