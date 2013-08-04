default: all
.PHONY: all clean

MAKE=make
CC=gcc
CCFLAGS= -g -O2 -Wall
LDFLAGS= -lm
export

all:
	cd src && $(MAKE) all
	cd examples && $(MAKE) all

clean:
	cd src && $(MAKE) clean
	cd examples && $(MAKE) clean

# cd src && $(MAKE) 等价于 $(MAKE) -C src
