%{

/* Alexander Cabos
 * CS370 - Lab9.y
 * 12/2/22
 * Added reading terminal for -o and -d for opening a file and debugging
 * Calls to emit.c to create gas code
 * 
 *-------------------------------------------------------------------------
 * 
 * Alexander Cabos
 * CS370 - Lab5.y
 * 10/9/22
 * As of lab 5, the yacc file now adds the linecount to any error messages.
 * tokens have been added to match those added in LEX
 * new rules have been entered on how to parse the program.
 *
 *--------------------------------------------------------------------------
 *
 * Alexander Cabos
 * 10/9/22
 * Lab6
 * lab6.y
 *
 * Purpose of lab6.y has changed to reading all input from lex, parsing it
 *	and sending that data to the abstract syntax tree (ast.c) to print out
 *  the input data in a more readable format.
 */




/* begin specs */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "ast.h" 
#include "symtable.h"
#include "emit.h"

int yylex();        
extern int debugsw; /* import the debugsw from lab5.l */
extern int linecount; /* import the linecount from lab5.l */

// global variable for the AST here

// level of blocks seen
int level = 0;

// current offset size
int offset = 0;
int maxoffset;
int goffset;


struct ASTnodetype *Program;

void yyerror (s)  /* Called by yyparse on error */
     char *s;
{
  printf ("%s, line number %d\n", s, linecount); /* added linecount to error message */
}

%}


/*  defines the start symbol, what values come back from LEX and how the operators are associated  */
%start Program

/* union allows yacc to recognize numbers and strings */
%union{
	int value; /* numbers */
	char* string; /* strings */
	struct ASTnodetype *node; /* Abstract Syntax Tree nodes */
	enum AST_Decaf_Types declared_type; /* AST node types */
	enum AST_Operators operator; /* AST operators, +-/etc */
}

/* Token declarations */
%token T_AND            		 /* && */
%token T_ASSIGN         		 /* = */
%token T_BOOLTYPE       		 /* bool */
%token T_BREAK          		 /* break */
%token T_CHARCONSTANT            /* char_lit */
%token T_CONTINUE       		 /* continue */
%token T_DOT            		 /* . */
%token T_ELSE           		 /* else */
%token T_EQ             		 /* == */
%token T_EXTERN         		 /* extern */
%token <value> T_FALSE           /* false */
%token T_FOR					 /* for */
%token T_FUNC           		 /* func */
%token T_GEQ            		 /* >= */
%token T_GT             		 /* > */
%token <string> T_ID    		 /* identifier */
%token T_IF             		 /* if */
%token <value> T_INTCONSTANT     /* int_lit */
%token T_INTTYPE        		 /* int */
%token T_LEFTSHIFT      		 /* << */
%token T_LEQ            		 /* <= */
%token T_LT						 /* less than */
%token T_NEQ            		 /* != */
%token T_NULL           		 /* null */
%token T_OR             		 /* || */
%token T_PACKAGE        		 /* package */
%token T_RETURN         		 /* return */
%token T_RIGHTSHIFT     		 /* >> */
%token <string> T_STRINGCONSTANT /* string_lit */
%token T_STRINGTYPE     		 /* string */
%token <value> T_TRUE            /* true */
%token T_VAR            		 /* var */
%token T_VOID           	 	 /* void */
%token T_WHILE          	 	 /* while */

/* Assigns types to all rules in yacc */
%type <node> Assign AdditiveExpression ArrayType Block BoolConstant Constant Expr Externs ExternDefn
%type <node> ExternTypeList ExternTypeList1 Factor FieldDecl FieldDecls Lvalue MethodArg MethodArgList
%type <node> MethodArgList1 MethodCall MethodDecl MethodDecls MethodTypeList MethodTypeList1
%type <node> SimpleExpression Statement Statements Term VarDecl VarDecls IfStmt WhileStmt ReturnStmt
%type <node> BreakStmt ContinueStmt
%type <declared_type> MethodType Type ExternType
%type <operator> Addop Relop Multop


%%	/* end specs, begin rules */

/* Starts program, the rest of the program branches from here */
Program : Externs T_PACKAGE T_ID '{' FieldDecls MethodDecls '}'
			{ 
				Program = ASTCreateNode(A_PROGRAM);
			  	Program -> S1 = $1;
			  	Program -> S2 = ASTCreateNode(A_PACKAGE);
				Program -> S2 -> symbol = Insert($3, A_Decaf_INT, ID_Sub_Type_Package, level, 0, 0, NULL);
			  	Program -> S2 -> name = $3;
				Program -> name = $3;
			  	Program -> S2 -> S1 = $5;
			 	Program -> S2 -> S2 = $6;
			}
		;
/* external functions/libraries */
Externs : /* empty */ { $$ = NULL; }
		| ExternDefn Externs
		 {
			$$ = $1;
		    $$ -> next = $2; 
		 }
		;
/* definition of an external function */
ExternDefn  : T_EXTERN T_FUNC T_ID '(' ExternTypeList ')' MethodType ';'
			 { 
				if(Search($3, level, 0) != NULL){
					yyerror("Variable already defined");
					yyerror($3);
					exit(1);
				}
				$$ = ASTCreateNode(A_EXTERN);
				$$ -> symbol = Insert($3, $7, ID_Sub_Type_Extern, level, 0, 0, $5);
			 	$$ -> name = $3; 
				$$ -> S1 = $5;
				$$ -> declared_type = $7;
				if(debugsw == 1) Display();
			 }
			;

/* ExternType, TypeList, and TypeList1 are used for getting all types from the externdefn*/
ExternTypeList  : /* empty */ { $$ = NULL; }
				| ExternTypeList1 { $$ = $1; }
				;

ExternTypeList1 : ExternType 
 				  { $$ = ASTCreateNode(A_EXTERN_TYPE);
					$$ -> declared_type = $1;
				  }
				| ExternType ',' ExternTypeList1
				  { $$ = ASTCreateNode(A_EXTERN_TYPE);
					$$ -> declared_type = $1;
					$$ -> next = $3; 
				  }
				;

ExternType  : T_STRINGTYPE {$$ = A_Decaf_STRING;}
			| Type {$$ = $1;}
			;

/* declares variable fields */
FieldDecls  : /* empty */ { $$ = NULL; }
			| FieldDecl FieldDecls
			  { $$ = $1;
			    $$ -> next = $2;
			  }
			;

FieldDecl : T_VAR T_ID Type ';' 
			{ 
				if(Search($2, level, 0) != NULL){
					yyerror("Variable already defined");
					yyerror($2);
					exit(1);
				}
				$$ = ASTCreateNode(A_VARDEC); 
			  	$$ -> name = $2; 
			  	$$ -> declared_type = $3; 
				$$ -> symbol = Insert($2, $3, ID_Sub_Type_Scalar, level, 1, offset, NULL);
				offset += 1;

			}
		  | T_VAR T_ID Type T_ASSIGN Constant ';'
		    { 
				if(Search($2, level, 0) != NULL){
					yyerror("Variable already defined");
					yyerror($2);
					exit(1);
				}
				$$ = ASTCreateNode(A_VARDEC);
				$$ -> name = $2; 
				$$ -> declared_type = $3; 
				$$ -> size = 1;
				$$ -> symbol = Insert($2, $3, ID_Sub_Type_Scalar, level, $$ -> size, offset, NULL);
				offset += 1;
				$$ -> S2 = $5;

			}
		  | T_VAR T_ID ArrayType ';'
		    { 
				if(Search($2, level, 0) != NULL){
					yyerror("Variable already defined");
					yyerror($2);
					exit(1);
				}
				$$ = ASTCreateNode(A_VARDEC); 
				$$ -> name = $2; 
				$$ -> S1 = $3;
				$$ -> declared_type = $3 -> declared_type;
				$$ -> value = $3 -> value;
				$$ -> size = $3 -> size;
				$$ -> symbol = Insert($2, $$ -> declared_type, ID_Sub_Type_Array, level, $$ -> size, offset, NULL);
				offset += $$ -> size;
			}
		  ;

/* declaration rules for methods */
MethodDecls : /* empty */ { $$ = NULL; }
			| MethodDecl MethodDecls
			  { $$ = $1;
			    $$ -> next = $2;
			  }
			;

MethodDecl : T_FUNC T_ID 
				{ 
					goffset = offset;
					struct SymbTab *s;
					s = Search($2, level, 0);
					if(s != NULL){
						yyerror("Method name already used");
						yyerror($2);
						exit(1);
					}
					s = Insert($2, 0, ID_Sub_Type_Method, level, 0, 0, NULL);
					offset = 0;
				} 
			 '(' MethodTypeList ')' MethodType 
				{
					struct SymbTab *s;
					s = Search($2, level, 0);
					s -> Type = $7;
					s -> fparms = $5;
				} 
			Block
			 	{ 
				$$ = ASTCreateNode(A_METHODDEC); 
				$$ -> symbol = Search($2, level, 0);
				$$ -> symbol -> mysize = maxoffset;
			   	$$ -> name = $2;
			    $$ -> declared_type = $7;
				$$ -> size = maxoffset;
			    $$ -> S1 = $5; 
			    $$ -> S2 = $9; 
			 }
		   ;

MethodTypeList : /* empty */ { $$ = NULL; }
			   | MethodTypeList1 { $$ = $1;}
			   ;

MethodTypeList1 : T_ID MethodType
				  {
					if(Search($1, level+1, 0) != NULL){
						yyerror("Duplicate MethodARG Name ");
						yyerror($1);
						exit(1);
					}
					$$ = ASTCreateNode(A_PARAM); // A_METHODID maybe
					$$ -> name = $1;
					$$ -> declared_type = $2;
					$$ -> symbol = Insert($1, $2, ID_Sub_Type_Scalar, level + 1, 1, offset, NULL);
					offset += 1;
				  }
				| T_ID MethodType ',' MethodTypeList1
				  {
					if(Search($1, level+1, 0) != NULL){
						yyerror("Duplicate MethodARG Name ");
						yyerror($1);
						exit(1);
					}
					$$ = ASTCreateNode(A_PARAM); // A_METHODID maybe
					$$ -> name = $1;
					$$ -> declared_type = $2;
					$$ -> next = $4;
					$$ -> symbol = Insert($1, $2, ID_Sub_Type_Scalar, level + 1, 1, offset, NULL);
					offset += 1;
					if(debugsw == 1) Display();
				  }
				;

/* types for methods */
MethodType  : T_VOID { $$ = A_Decaf_VOID; }
			| Type { $$ = $1; }
			;

/* variable/method/etc types */
Type : T_INTTYPE { $$ = A_Decaf_INT; }
	 | T_BOOLTYPE { $$ = A_Decaf_BOOL; }
	 ;

/* arrays must be handled separately, since they include a constant */
ArrayType : '[' T_INTCONSTANT ']' Type
			{ $$ = ASTCreateNode(A_ARRAYTYPE);
			  $$ -> value = $2;
			  $$ -> size = $2;
			  $$ -> declared_type = $4;
			}
		  ;

/* how to treat constants */
Constant : T_INTCONSTANT 
		   { 
			 $$ = ASTCreateNode(A_CONSTANT_INT);
			 $$ -> value = $1; 
			 $$ -> declared_type = A_Decaf_INT;
		   }
		 | BoolConstant { $$ = $1; }
		 ;

BoolConstant : T_TRUE 
			   { 
				 $$ = ASTCreateNode(A_CONSTANT_BOOL);
				 $$ -> value = 1; 
				 $$ -> declared_type = A_Decaf_BOOL;
			   }
			 | T_FALSE 
			   { $$ = ASTCreateNode(A_CONSTANT_BOOL);
			     $$ -> value = 0; 
				 $$ -> declared_type = A_Decaf_BOOL;
			   }
			 ;

/* all declarations and statements within a function's body */
Block : '{' {level++;} VarDecls Statements '}'
		{ 
			$$ = ASTCreateNode(A_BLOCK);
			$$ -> S1 = $3;
			$$ -> S2 = $4;
			if(debugsw == 1) Display();
			if(offset > maxoffset){ maxoffset = offset;	}
			offset -= Delete(level);
			level--;
		}
	  ;

/* how to declare variables */
VarDecls : /* empty */ { $$ = NULL; }
		 | VarDecl VarDecls
		   {
			 $$ = $1;
			 $$ -> next = $2;
		   }
		 ;

VarDecl : T_VAR T_ID Type ';'
		  {
			if(Search($2, level, 0) != NULL){
				// it was in the symbol table
				yyerror("Variable already defined");
				yyerror($2);
				exit(1);
			}
			$$ = ASTCreateNode(A_VARDEC);
			$$ -> name = $2;
			$$ -> declared_type = $3;
			$$ -> size = 1;
			$$ -> symbol = Insert($2, $3, ID_Sub_Type_Scalar, level, $$ -> size, offset, NULL);
			offset += 1;
		  }
		| T_VAR T_ID ArrayType ';'
		  {
			if(Search($2, level, 0) != NULL){
				// it was in the symbol table
				yyerror("Variable already defined");
				yyerror($2);
				exit(1);
			}
			$$ = ASTCreateNode(A_VARDEC);
			$$ -> name = $2;
			$$ -> S1 = $3;
			$$ -> declared_type = $3 -> declared_type;
			$$ -> size = $3 -> size;
			$$ -> symbol = Insert($2, $3 -> declared_type, ID_Sub_Type_Array, level, $$ -> size, offset, NULL);
			offset += $3 -> size;
		  }
		;

/* rules on what is a statement and how to treat them */
Statements : /* empty */ { $$ = NULL; }
		   | Statement Statements
		     {
				$$ = $1;
				$$ -> next = $2;
			 }
		   ;

Statement : Block { $$ = $1; }
		  | Assign ';' { $$ = $1; } 
		  | MethodCall ';' { $$ = $1; }
		  | IfStmt { $$ = $1; }
		  | WhileStmt { $$ = $1; }
		  | ReturnStmt { $$ = $1; }
		  | BreakStmt { $$ = $1; }
		  | ContinueStmt { $$ = $1; }
		  ;

/* if/else statements*/
IfStmt : T_IF '(' Expr ')' Block
		{ 
		  	$$ = ASTCreateNode(A_IF);
			$$ -> S1 = $3;
			$$ -> S2 = ASTCreateNode(A_BODY);
		  	$$ -> S2 -> S1 = $5;
			$$ -> S2 -> S2 = NULL;
		  	
		}
	   | T_IF '(' Expr ')' Block T_ELSE Block
		{ 
			$$ = ASTCreateNode(A_IF);
			$$ -> S1 = $3;
		  	$$ -> S2 = ASTCreateNode(A_BODY);
		  	$$ -> S2 -> S1 = $5;
		  	$$ -> S2 -> S2 = $7;
		}
	   ;


/* while statement */
WhileStmt : T_WHILE '(' Expr ')' Block 
			{ 
				$$ = ASTCreateNode(A_WHILE);
				$$ -> S1 = $3;
				$$ -> S2 = $5;
			}
          ;
/* return statement */
ReturnStmt : T_RETURN ';' { $$ = ASTCreateNode(A_RETURN); }
		   | T_RETURN '(' ')' ';' { $$ = ASTCreateNode(A_RETURN); }
		   | T_RETURN '(' Expr ')' ';' 
		  	{
				$$ = ASTCreateNode(A_RETURN);
				$$ -> S1 = $3;
			}
		  ;
/* break statement */
BreakStmt : T_BREAK ';' { $$ = ASTCreateNode(A_BREAK); }
		  ;
/* continue statement*/
ContinueStmt : T_CONTINUE ';' { $$ = ASTCreateNode(A_CONTINUE); }
			 ;


Assign : Lvalue T_ASSIGN Expr 
		{ 
			if( $1 -> declared_type != $3 -> declared_type ){
				yyerror("Type mismatch on assignment");
				exit(1);
			}
			$$ = ASTCreateNode(A_ASSIGN);
			$$ -> S1 = $1;
			$$ -> S2 = $3;
			$$ -> symbol = Insert(TEMP_CREATE(), $3 -> declared_type, ID_Sub_Type_Scalar, level, 1, offset++, NULL);
			
		}
	   ;
/* values left of the assignment operator */
Lvalue : T_ID 
		{ 
			struct SymbTab *p;
			p = Search($1, level, 1);
			if(p == NULL){
				yyerror("Symbol on Lvalue not defined");
				yyerror($1);
				exit(1);
			}
			if(p -> SubType != ID_Sub_Type_Scalar){
				yyerror("Type mismatch, variable not scalar");
				yyerror($1);
				exit(1);
			}

			$$ = ASTCreateNode(A_VAR_LVALUE);
			$$ -> name = $1; 
			$$ -> symbol = p;
			$$ -> declared_type = p -> Type;
		}
	   | T_ID '[' Expr ']' 
	     { 
			struct SymbTab *p;
			p = Search($1, level, 1);
			if(p == NULL){
				yyerror("Symbol on Lvalue not defined");
				yyerror($1);
				exit(1);
			}
			if(p -> SubType != ID_Sub_Type_Array){
				yyerror("Type mismatch, variable not array");
				yyerror($1);
				exit(1);
			}
			
			$$ = ASTCreateNode(A_VAR_LVALUE);
			$$ -> name = $1; 
			$$ -> S1 = $3;
			$$ -> symbol = p;
			$$ -> declared_type = p -> Type;
		 }
	   ;
/* rules on how to call a method and how to treat it's arguments */
MethodCall : T_ID '(' MethodArgList ')' 
			 { 
				struct SymbTab *p;
				p = Search($1, level, 1);
				if(p == NULL){
					yyerror("Symbol not defined");
					yyerror($1);
					exit(1);
				}
				if( (p -> SubType != ID_Sub_Type_Method) && (p -> SubType != ID_Sub_Type_Extern) ){
					yyerror("Type mismatch, needs to be a method");
					yyerror($1);
					exit(1);
				}
				if( check_parameters( p -> fparms, $3 ) == 0 ){
					yyerror("Formal and actual parameters do not match");
					yyerror($1);
					exit(1);
				}
				$$ = ASTCreateNode(A_METHODCALL);
				$$ -> name = $1;
				$$ -> S1 = $3;
				$$ -> symbol = p;
				$$ -> declared_type = p -> Type;
			 }
		   ;

MethodArgList : /* empty */ { $$ = NULL; }
			  | MethodArgList1 { 
					$$ = $1;
			  }
			  ;

MethodArgList1 : MethodArg { $$ = $1; }
			   | MethodArg ',' MethodArgList1 
			     { 
					$$ = $1;
					$$ -> next = $3; 
				 }
			   ;
		   
MethodArg : Expr 
			{ 
				$$ = ASTCreateNode(A_METHODARG);
				$$ -> S1 = $1; 
				$$ -> value = $1 -> value;
				$$ -> declared_type = $1 -> declared_type;
				$$ -> name = TEMP_CREATE();
				$$ -> symbol = Insert( $$ -> name, $$ -> declared_type, ID_Sub_Type_Scalar, level, 1, offset++, NULL );
			}
		  | T_STRINGCONSTANT
		    {
				$$ = ASTCreateNode(A_METHODARG);
				$$ -> S1 = ASTCreateNode(A_CONSTANT_STRING);
				$$ -> S1 -> name = $1;
				$$ -> declared_type = A_Decaf_STRING;
				$$ -> name = TEMP_CREATE();
				$$ -> symbol = Insert( $$ -> name, $$ -> declared_type, ID_Sub_Type_Scalar, level, 1, offset++, NULL );
			}
		  ;


/* how to parse an expression, separated by order of operations */
Expr : SimpleExpression { $$ = $1; } 
	 ;

SimpleExpression : AdditiveExpression { $$ = $1; }
				 | SimpleExpression Relop AdditiveExpression
				   {
					if( $1 -> declared_type != $3 -> declared_type ){
						yyerror("Both sides need to be the same type");
						exit(1);
					}
					$$ = ASTCreateNode(A_EXPR);
					$$ -> S1 = $1;
					$$ -> operator = $2;
					$$ -> name = TEMP_CREATE();
					$$ -> S2 = $3;
					$$ -> declared_type = A_Decaf_BOOL;
					$$ -> symbol = Insert( $$ -> name, $$ -> declared_type, ID_Sub_Type_Scalar, level, 1, offset++, NULL );
				   }
				 ;

Relop : T_LEQ { $$ = A_LEQ; }
	  | T_LT  { $$ = A_LT;  }
	  | T_GT  { $$ = A_GT;  }
	  | T_GEQ { $$ = A_GEQ; }
	  | T_EQ  { $$ = A_EQ;  }
	  | T_NEQ { $$ = A_NEQ; }
      ;

AdditiveExpression : Term { $$ = $1; }
                   | AdditiveExpression Addop Term 
				     {
						if( $1 -> declared_type != $3 -> declared_type ){
							yyerror("Type Mismatch");
							exit(1);
						}
						if(( $1 -> declared_type != A_Decaf_INT )){
							yyerror("Must use integers in addition/subtraction");
							exit(1);
						}
						$$ = ASTCreateNode(A_EXPR);
						$$ -> S1 = $1;
						$$ -> operator = $2;
						$$ -> name = TEMP_CREATE();
						$$ -> S2 = $3;
						$$ -> declared_type = $1 -> declared_type;
						$$ -> symbol = Insert( $$ -> name, $$ -> declared_type, ID_Sub_Type_Scalar, level, 1, offset++, NULL );
					 }
                   ;


Addop : '+' { $$ = A_PLUS; }
	  | '-' { $$ = A_MINUS; }
      ;

Term : Factor { $$ = $1; }
     | Term Multop Factor 
	   {
		 if( $1 -> declared_type != $3 -> declared_type ){
			yyerror("Type Mismatch");
			exit(1);
		 }
		 if(( $1 -> declared_type == A_Decaf_BOOL ) && 
		    ( $2 == A_TIMES || $2 == A_DIVIDE || $2 == A_MOD )){
			yyerror("Cannot use boolean in arethmatic operations");
			exit(1);
		 }
		 if(( $1 -> declared_type == A_Decaf_INT ) && 
		    ( $2 == A_AND || $2 == A_OR || $2 == A_LEFTSHIFT || $2 == A_RIGHTSHIFT )){
			yyerror("Cannot use integer in logical operations");
			exit(1);
		 }
		 $$ = ASTCreateNode(A_EXPR);
		 $$ -> S1 = $1;
		 $$ -> operator = $2;
		 $$ -> declared_type = $1 -> declared_type;
		 $$ -> S2 = $3;
		 $$ -> name = TEMP_CREATE();
		 $$ -> symbol = Insert( $$ -> name, $$ -> declared_type, ID_Sub_Type_Scalar, level, 1, offset++, NULL );
	   }
     ;

Multop : '*' { $$ = A_TIMES; }
	   | '/' { $$ = A_DIVIDE; }
	   | '%' { $$ = A_MOD; }
	   | T_AND { $$ = A_AND; }
	   | T_OR { $$ = A_OR; }
	   | T_LEFTSHIFT { $$ = A_LEFTSHIFT; }
	   | T_RIGHTSHIFT { $$ = A_RIGHTSHIFT; }
       ;

/* terminals for expressions*/
Factor : T_ID
		 {
			struct SymbTab *p;
			p = Search($1, level, 1);
			if(p == NULL){
				yyerror("Symbol on Rvalue not defined");
				yyerror($1);
				exit(1);
			}
			if(p -> SubType != ID_Sub_Type_Scalar){
				yyerror("Type mismatch, variable not scalar");
				yyerror($1);
				exit(1);
			}
			$$ = ASTCreateNode(A_VAR_RVALUE);
			$$ -> name = $1;
			$$ -> symbol = p;
			$$ -> declared_type = p -> Type;
		 }
       | MethodCall { $$ = $1; } 
       | T_ID '[' Expr ']'
	     {
			struct SymbTab *p;
			p = Search($1, level, 1);
			if(p == NULL){
				yyerror("Symbol on Lvalue not defined");
				yyerror($1);
				exit(1);
			}
			if(p -> SubType != ID_Sub_Type_Array){
				yyerror("Type mismatch, variable not array");
				yyerror($1);
				exit(1);
			}
			$$ = ASTCreateNode(A_VAR_RVALUE);
			$$ -> name = $1;
			$$ -> S1 = $3;
			$$ -> symbol = p;
			$$ -> declared_type = p -> Type;
			
		 }
       | Constant { $$ = $1; }
       | '(' Expr ')' { $$ = $2; }
       | '!' Factor 
	     {
			if($2 -> declared_type != A_Decaf_BOOL){
				yyerror("Type mismatch, expecting boolean");
				exit(1);
			}
			$$ = ASTCreateNode(A_EXPR);
			$$ -> operator = A_NOT;
			$$ -> S1 = $2;
			$$ -> declared_type = A_Decaf_BOOL;
			$$ -> name = TEMP_CREATE();
			$$ -> symbol = Insert( $$ -> name, A_Decaf_BOOL, ID_Sub_Type_Scalar, level, 1, offset++, NULL );
		 }
       | '-' Factor
	     {
			if( $2 -> declared_type != A_Decaf_INT ){
				yyerror("Type mismatch, expecting integer");
				exit(1);
			}
			
			$$ = ASTCreateNode(A_EXPR);
			$$ -> operator = A_UMINUS;
			$$ -> S1 = $2;
			$$ -> name = TEMP_CREATE();
			$$ -> symbol = Insert( $$ -> name, A_Decaf_INT, ID_Sub_Type_Scalar, level, 1, offset++, NULL );
		 }
       ;

%%	/* end of rules, start of program */


int main( int argc, char *argv[] ){ 
	int i = 1;
	FILE *fp;
	char s[100];

	/* read terminal */
	while( i < argc ){
		/* check for '-d' for debug */
		if( strcmp( argv[i], "-d" ) == 0 ) { debugsw = 1; }
		/* check for '-o' for opening a file */
		if( strcmp( argv[i], "-o" ) == 0 ){
			strcpy( s, argv[i+1] );
			strcat( s, ".s" );
			if( (fp = fopen(s, "w")) == NULL  ){
				printf( "cannot open %s\n", argv[i+1] );
				exit(1);
			}
		}
		i = i + 1;
	}

	yyparse(); /* continues running so long as there is valid input */
	if(debugsw == 1) Display(); /* display from symtable.c showing detailed info about all variables/funcs/etc */
	if(debugsw == 1) printf("Parsing completed\n"); /* indicate program has begun */
	if(debugsw == 1) ASTprint(0, Program); /* Start parsing input */
	if(debugsw == 1) printf("\nFinished printing AST\n"); /* indicate that ASTprint() has completed*/
	EMIT( Program, fp ); /* create gas code */
}

