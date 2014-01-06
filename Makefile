###Makefile for mars map project


CC=gcc
CFLAGS=-O0 -Wall

mars: map.o map.h map.c main.c
	$(CC) $(CFLAGS) main.c map.o -o mars `imlib2-config --cflags` `imlib2-config --libs`

map.o:
	$(CC) $(CFLAGS) -c map.c
clean:
	@ rm *.o
	@ rm mars
