# Alexander Cabos
# CS 474 - Operating Systems
# Makefile for Program 6

# Variables
CC = gcc
CFLAGS = -Wall -g 

# Targets
all: myfile

# Dependencies
myfile.o: myfile.c
	$(CC) $(CFLAGS) -c myfile.c

myfile: myfile.o
	$(CC) $(CFLAGS) myfile.o -o myfile

# Clean up
clean:
	rm -f myfile myfile.o