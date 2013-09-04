default: all

.PHONY: all clean share static

SOURCE = $(wildcard *.c)
share:$(patsubst %.c,lib%.so,$(SOURCE))
static:$(patsubst %.c,lib%.a,$(SOURCE))
CC = gcc -g -Wall -O2

all:static

%.o:%.c
	$(CC) -c $< -I../include

lib%.a:%.o
	@ar -crv $@ $^

lib%.so:%.o
	gcc -shared -fPIC -o $@ $^

clean:
	rm -f *.o *.a *.so

# @ hidden the output of commands
