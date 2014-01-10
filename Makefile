###Makefile for mars map project


CC=gcc
CFLAGS=-O0 -Wall -g
LFLAGS= -lpthread -lImlib2

mars: map.o main.c map.h pyramid.h construct.h pyramid.o construct.o
	$(CC) $(CFLAGS) main.c map.o pyramid.o construct.o -o mars $(LFLAGS)

map.o: map.c
	$(CC) $(CFLAGS) -c map.c
pyramid.o: pyramid.c
	$(CC) $(CFLAGS) -c pyramid.c

construct.o: construct.c
	$(CC) $(CFLAGS) -c construct.c
clean:
	@ rm *.o
	@ rm mars
