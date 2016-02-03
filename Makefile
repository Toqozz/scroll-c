CC=gcc

CFLAGS=-Wall -g
all:	scroll

scroll:
	$(CC) $(CFLAGS)	scroll.c	-o scroll
clean:
	rm -f scroll
