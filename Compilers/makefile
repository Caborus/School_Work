# Alexander Cabos
# CS 370
# 12/2/22
# Lab 9 makefile

all: lab9

lab9: lab9.l lab9.y ast.c ast.h symtable.c symtable.h trial.decaf emit.c emit.h
	yacc -d lab9.y
	lex lab9.l
	gcc -o lab9 lex.yy.c y.tab.c ast.c symtable.c emit.c
#	~scooper/lab9 -o small < trial.decaf
	
run: lab9 trial.decaf 
	./lab9 -o trial < trial.decaf
	gcc -o trial trial.s
	./trial
	
clean:
	rm -f lab9