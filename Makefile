CC=gcc
CFLAGS=-O9
LDFLAGS=
EXEC=main

all: $(EXEC)

main: bisection.o main.o
	$(CC) -o cut2order bisection.o main.o $(CFLAGS)

bisection.o: bisection.c
	$(CC) -o bisection.o -c bisection.c $(CFLAGS)

main.o: main.c
	$(CC) -o main.o -c main.c $(CFLAGS)

clean:
	rm *.o cut2order

