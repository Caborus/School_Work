/*  Alexander Cabos
 12/2/22
 Lab9
 ast.h

 Added label to ASTnode for use in emit.c
 
 -----------------------------------------------------------

 Abstract syntax tree code


 Header file   
 Shaun Cooper January 2020


 ------------------------------------------------------------

 Alexander Cabos
 10/9/22
 Lab6
 ast.h

 Header file updated with more AST_Tree_Element_Types to allow more abstract functions
   to pass from yacc to ast.c
 More operators added to allow for processing different expressions
 Added String to AST_Decaf_Types to allow for processing string type values.
 Added declared_type to hold the type value of a node.

*/

#include<stdio.h>
#include<malloc.h>

#ifndef AST_H
#define AST_H
static int debugsw;

/* define the enumerated types for the AST.  THis is used to tell us what 
sort of production rule we came across */

enum AST_Tree_Element_Type {
   A_PROGRAM,
   A_PACKAGE,
   A_EXTERN,
   A_EXTERN_TYPE,
   A_METHODCALL,
   A_METHODDEC,
   A_NUMBER,
   A_BLOCK,
   A_EXPR,
   A_PARAM,
   A_VARDEC,
   A_ARRAYTYPE,
   A_CONSTANT_INT,
   A_CONSTANT_BOOL,
   A_CONSTANT_STRING,
   A_BREAK,
   A_VAR_LVALUE,
   A_VAR_RVALUE,
   A_RETURN,
   A_CONTINUE,
   A_IF,
   A_WHILE,
   A_ASSIGN,
   A_METHODARG,
   A_ID,
   A_BODY
};


enum AST_Operators {
   A_PLUS,
   A_MINUS,
   A_TIMES,
   A_DIVIDE,
   A_MOD,
   A_AND,
   A_OR,
   A_LEFTSHIFT,
   A_RIGHTSHIFT,
   A_UMINUS,
   A_LEQ,
   A_LT,
   A_GT,
   A_GEQ,
   A_EQ,
   A_NEQ,
   A_NOT
};

enum AST_Decaf_Types {
   A_Decaf_INT,
   A_Decaf_BOOL,
   A_Decaf_VOID,
   A_Decaf_STRING
};

/* define a type AST node which will hold pointers to AST structs that will
   allow us to represent the parsed code 
*/
typedef struct ASTnodetype
{
   enum AST_Tree_Element_Type type;
   enum AST_Operators operator;
   enum AST_Decaf_Types declared_type;
   char * name;
   char * label;
   int value;
   struct SymbTab * symbol;
   struct ASTnodetype *S1,*S2, *next ; /* used for holding IF and WHILE components -- not very descriptive */
   int size;

} ASTnode;

#include "symtable.h"

/* uses malloc to create an ASTnode and passes back the heap address of the newley created node */
ASTnode *ASTCreateNode(enum AST_Tree_Element_Type mytype);

void PT(int howmany); // prints howmany spaces


ASTnode *program;  // Global Pointer for connection between YACC and backend

/*  Print out the abstract syntax tree */
void ASTprint(int level,ASTnode *p); // prints tree with level horizontal spaceing

int check_parameters(ASTnode *formal, ASTnode *actual);

#endif // of AST_H
