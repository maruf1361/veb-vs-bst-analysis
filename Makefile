CC=gcc
CFLAGS=-Wall -Wextra -g -I.
LIBS=-lm

all: compare

compare: main.o veb.o bst.o
	$(CC) $(CFLAGS) -o compare main.o veb.o bst.o $(LIBS)

main.o: main.c veb.h bst.h
	$(CC) $(CFLAGS) -c main.c

veb.o: veb.c veb.h
	$(CC) $(CFLAGS) -c veb.c

bst.o: bst.c bst.h
	$(CC) $(CFLAGS) -c bst.c

clean:
	rm -f *.o compare