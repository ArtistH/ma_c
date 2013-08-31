default: all

.PHONY: all clean lib

SOURCE = $(wildcard *.c)
TARGET = $(patsubst %.c,%.bin,$(SOURCE))
CC = gcc -g -Wall -O2

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

ex-1.bin:libgraph.a liblist.a libset.a
ex-2.bin:libbfs.a libgraph.a liblist.a libset.a libqueue.a
ex-3.bin:libdfs.a libgraph.a liblist.a libset.a
