###Makefile for mars map project


CC=gcc
CFLAGS=-O0 -Wall

mars: main.o map.o
	$(CC) $(CFLAGS) main.o images.o -o mars

main.o:
	$(CC) $(CFLAGS) -c main.c

map.o:
	$(CC) $(CFLAGS) -c map.c
