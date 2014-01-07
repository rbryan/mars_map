###Makefile for mars map project


CC=gcc
CFLAGS=-O0 -Wall

mars: map.o map.h map.c main.c
	$(CC) $(CFLAGS) main.c map.o -o mars 

map.o: map.c
	$(CC) $(CFLAGS) -c map.c
clean:
	@ rm *.o
	@ rm mars
