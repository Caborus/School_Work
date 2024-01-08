# Alexander Cabos
# CS 474 - Operating Systems
# 10/15/2023
# Makefile for programming assignment 3

# Variables
CC = gcc
CFLAGS = -Wall -g -lpthread

# Targets
all: normal shared thread

# Dependencies
normal.o: normal.c
	$(CC) $(CFLAGS) -c normal.c

normal: normal.o
	$(CC) $(CFLAGS) normal.o -o normal

shared.o: shared.c
	$(CC) $(CFLAGS) -c shared.c

shared: shared.o
	$(CC) $(CFLAGS) shared.o -o shared

thread.o: thread.c
	$(CC) $(CFLAGS) -c thread.c

thread: thread.o
	$(CC) $(CFLAGS) thread.o -o thread

# Clean remove normal executable and object files
clean:
	rm -f normal normal.o shared shared.o thread thread.o