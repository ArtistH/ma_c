default: all

.PHONY: all clean

SOURCE = $(wildcard *.c)
TARGET = $(patsubst %.c,lib%.a,$(SOURCE))
CC = gcc -g -Wall -O2

all:$(TARGET)

%.o:%.c
	$(CC) -c $< -I../include

lib%.a:%.o
	ar -crv $@ $^

lib%.so:%.o
	gcc -shared -fPIC -o $@ $^

clean:
	rm -f *.o *.a *.so
