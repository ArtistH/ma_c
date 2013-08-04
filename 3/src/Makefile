default: all
.PHONY: all clean

SOURCES=$(wildcard *.c)
EXES=
OBJS=$(patsubst %.c,%.o,$(SOURCES))

vpath %.h $(TOPDIR)/include/
vpath %.c $(TOPDIR)/src/

all: ${OBJS}

clean:
	rm -f ${EXES} ${OBJS} *.o
