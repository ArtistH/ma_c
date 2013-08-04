SOURCES=$(wildcard *.c)
OBJS=$(patsubst %.c,%,$(SOURCES))

vpath %.h $(TOPDIR)/include/

default: all
.PHONY: all clean

all:$(OBJS)
$(OBJS):%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJS) *.o
