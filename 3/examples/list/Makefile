SOURCES=$(wildcard *.c)
OBJS=$(patsubst %.c,%,$(SOURCES))

MAKE=make
CC=gcc
INCLUDES= -I ./../../include/
CCFLAGS= -g -O2 -Wall
LDFLAGS=

default: all
.PHONY: all clean

all:$(OBJS)
$(OBJS):
	$(CC) $< -o $@ $(INCLUDES) $(LDFLAGS)
%.o:%.c
	$(CC) -c $< $(CCFLAGS) $(INCLUDES)

clean:
	rm -f $(OBJS) *.o
