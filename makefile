CC=g++

CFLAGS=-O2 -march=native -flto -Wall -pipe -std=c++17

all: main.o mainprog exec

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

mainprog: main.o
	$(CC) -o mainprog main.o

exec: mainprog
	./mainprog

clean:
	rm *.o
	rm ./Data/* -f
