default: all

.PHONY: all clean

SOURCE = $(wildcard *.c)
TARGET = $(patsubst %.c,lib%.a,$(SOURCE))

all:$(TARGET)

%.o:%.c
	gcc -c $< -I../include

lib%.a:%.o
	ar -crv $@ $^

lib%.so:%.o
	gcc -shared -fPIC -o $@ $^

clean:
	rm -f *.o *.a *.so
