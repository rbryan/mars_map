###Makefile for mars map project


CC=gcc
CFLAGS=-O0 -Wall

mars: main.o map.o
	$(CC) $(CFLAGS) main.o map.o -o mars `imlib2-config --cflags` `imlib2-config --libs`

main.o:
	$(CC) $(CFLAGS) -c main.c

map.o:
	$(CC) $(CFLAGS) -c map.c
clean:
	@ rm *.o
	@ rm mars
