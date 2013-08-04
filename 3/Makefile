default: all
.PHONY: all clean

MAKE=make
CC=gcc
CFLAGS= -g -O -Wall
TOPDIR=$(shell pwd)
export

all:
	cd $(TOPDIR)/src && $(MAKE) all
	cd $(TOPDIR)/examples && $(MAKE) all

clean:
	cd $(TOPDIR)/src && $(MAKE) clean
	cd $(TOPDIR)/examples && $(MAKE) clean

# cd src && $(MAKE) 等价于 $(MAKE) -C src
