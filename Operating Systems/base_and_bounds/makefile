# Alexander Cabos
# CS 474 - Operating Systems
# 11/12/2023
# Makefile for programming assignment 5

# Variables
CC = gcc
CFLAGS = -Wall -g

# Targets
all: program5

# Dependencies
program5.o: program5.c
	$(CC) $(CFLAGS) -c program5.c

program5: program5.o
	$(CC) $(CFLAGS) program5.o -o program5


# Clean remove program5 executable and object files
clean:
	rm -f program5 program5.o 