# makefile for all Chapter 6 programs
CC = gcc
CFLAGS = -fopenmp

all: count3sPThread count3sOMP CountThrees

count3sPThread.o: count3sPThread.c
	$(CC) $(CFLAGS) -c count3sPThread.c

count3sPThread: count3sPThread.o
	$(CC) $(CFLAGS) count3sPThread.o -o count3sPThread

count3sOMP.o: count3sOMP.c
	$(CC) $(CFLAGS) -c count3sOMP.c

count3sOMP: count3sOMP.o
	$(CC) $(CFLAGS) count3sOMP.o -o count3sOMP

CountThrees: CountThrees.java
	javac CountThrees.java

clean: 
	@rm -f *.o *.class testOMP count3sOMP count3sPThread CountThrees