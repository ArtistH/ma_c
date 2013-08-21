default: all

.PHONY: all clean lib

SOURCE = $(wildcard *.c)
TARGET = $(patsubst %.c,%.bin,$(SOURCE))
CC = gcc -g -Wall -O2 -lm

VPATH = ../../src/

all:lib $(TARGET)

lib:
	$(MAKE) -C ../../src

%.o:%.c
	$(CC) -c $< -I../../include

%.bin:%.o
	$(CC) $^ -o $@

clean:
	rm -f *.o $(TARGET)

ex-1.bin:libfact.a libfacttail.a
ex-2.bin:libfactor.a
