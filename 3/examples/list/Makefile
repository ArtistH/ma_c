default: all

.PHONY: all clean lib

SOURCE = $(wildcard *.c)
TARGET = $(patsubst %.c,%.bin,$(SOURCE))

VPATH = ../../src/

all:lib $(TARGET)

lib:
	$(MAKE) -C ../../src

%.o:%.c
	gcc -c $< -I../../include

%.bin:%.o
	gcc $^ -o $@

clean:
	rm -f *.o $(TARGET)

ex-1.bin:liblist.a
