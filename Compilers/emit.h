/*
 * Alexander Cabos
 * CS 370 - emit.h
 * 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef EMIT_H
#define EMIT_H

#include "ast.h"
#include "symtable.h"

// word size
#define WSIZE     8
#define LOG_WSIZE 3

FILE *fp;

char *genlabel(); //needed in YACC for T_STRING_LIT

void EMIT( ASTnode * p, FILE *fp );
void Emit_Strings(ASTnode *p, FILE *fp);
void Emit_Globals(ASTnode *p, FILE *fp);
void Emit_AST(ASTnode *p, FILE *fp);
void Emit_Method_Decl(ASTnode *p, FILE *fp);
void Emit_Return(ASTnode * p, FILE *fp);
void Emit_Expr(ASTnode *p, FILE *fp);
void Emit_Method_Call(ASTnode *p, FILE *fp);
void Emit_Extern_Method_Call(ASTnode *p, FILE *fp);
char * CREATE_LABEL();
void Emit_Operator(ASTnode *p, FILE *fp);
void Emit_Return(ASTnode * p, FILE *fp);
void Emit_Assign(ASTnode *p, FILE *fp);
void Emit_If(ASTnode *p, FILE *fp);
void Emit_While(ASTnode *p, FILE *fp);

#endif