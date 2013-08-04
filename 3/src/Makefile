default: all
.PHONY: all clean

CC=gcc
CCFLAGS= -g -O2 -Wall
INCLUDES= -I ../include/

SOURCES=$(wildcard *.c)
EXES=
OBJS=$(patsubst %.c,%.o,$(SOURCES))

all: ${OBJS}

%.o: %.c
	$(CC) -c $< $(CCFLAGS) $(INCLUDES)

clean:
	rm -f ${EXES} ${OBJS} *.o
