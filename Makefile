###Makefile for mars map project


CC=gcc
CFLAGS=-O0 -Wall -g

mars: map.o main.c map.h pyramid.h pyramid.o
	$(CC) $(CFLAGS) main.c map.o pyramid.o -o mars 

map.o: map.c
	$(CC) $(CFLAGS) -c map.c
pyramid.o: pyramid.c
	$(CC) $(CFLAGS) -c pyramid.c
clean:
	@ rm *.o
	@ rm mars
