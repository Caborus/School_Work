/*  
    Alexander Cabos
    12/2/22
    Lab9
    ast.c

    Added labels to ASTNode for use in emit.c

    -----------------------------------------------------------------------------
    Abstract syntax tree code

    This code is used to define an AST node, 
    routine for printing out the AST
    defining an enumerated type so we can figure out what we need to
    do with this.  The ENUM is basically going to be every non-terminal
    and terminal in our language.

    Shaun Cooper February 2020

    ------------------------------------------------------------------------------
    Alexander Cabos
    10/9/22
    Lab6
    ast.c

    Updated PT() function to introduce tabbed formatting for output
    updated ASTprint_type() to take the type, and return the string name for 
      the appropriate type. No longer prints names on its own, but is more useful for 
      type checking in ASTprint
    updated ASTprint() to more accurately process the data from Yacc

*/

#include <stdio.h>
#include <malloc.h>
#include "ast.h" 
#include "symtable.h"


/* uses malloc to create an ASTnode and passes back the heap address of the newley created node */
ASTnode *ASTCreateNode( enum AST_Tree_Element_Type mytype )
{
    ASTnode *p;
    if( debugsw ) fprintf( stderr, "Creating AST Node \n" );
    p = ( ASTnode * )malloc( sizeof( ASTnode ) ); // get head data
    p -> type = mytype; // set up the Element type
    p -> S1 = NULL;
    p -> S2 = NULL;  // det default values
    p -> next = NULL;
    p -> value = 0;
    p -> label = NULL;
    return( p );
}

/*  Helper function to print tabbing */
// PRE: An int representing how many tabs should be inserted
// POST: a string of spaces representing the input number of tabs
void PT( int howmany )
{
  for( int i = 0; i < howmany; i++ ){
    printf( "  " );
  }
}


// ASTPrint_type(enum)
//  PRE: a enum TYPE
//  POST: string representing the type
char * ASTprint_type( enum AST_Decaf_Types intype ){
  switch(intype){
    case A_Decaf_INT:     return ( "INT"     );
                          break;
    case A_Decaf_BOOL:    return ( "BOOLEAN" );
                          break;
    case A_Decaf_VOID:    return ( "VOID"    ); 
                          break;
    case A_Decaf_STRING:  return ( "STRING"  );
                          break;
    default:              return( "Unknown type in ASTprint_type" );
  } // end switch
} // end ASTprint_type


/*  Print out the abstract syntax tree */
// PRE: A node from Yacc
// POST: print output of each function to the screen.
void ASTprint( int level, ASTnode *p ) {
  int i;
  if( p == NULL ) return;
  else{   
    PT( level ); /*indent */
    switch ( p -> type ){
      // top level program
      case A_PROGRAM :      printf( "PROGRAM\n" );
                            ASTprint( level, p -> S1 );
                            ASTprint( level, p -> S2 );
                            break;
      // body of A_PROGRAM
      case A_PACKAGE :      printf( "Package : %s\n\n", p -> name );
                            ASTprint( level + 1, p -> S1 );
                            ASTprint( level + 1, p -> S2 );
                            break;
      // External functions
      case A_EXTERN :       printf( "EXTERN FUNC %s\n", p -> name, ASTprint_type( p -> declared_type ) );
                              //PT(level+1);
                              //printf("EXTERN Type: \n");
                              ASTprint( level + 1, p -> S1 );
                              PT( level + 1 );
                              printf( "END EXTERN with Type: %s\n\n", ASTprint_type( p -> declared_type ) );
                              break;
      // Type of Extern function            
      case A_EXTERN_TYPE :    printf( "EXTERN Type: %s\n", ASTprint_type( p -> declared_type ) );
                              break;
      // Variable declarations
      case A_VARDEC :         printf( "Variable %s offset %d ", p -> name, p -> symbol -> offset );
                              if( p -> S1 != NULL ){
                                  printf( "[%d] ", p -> S1 -> value );
                              }


                              if( p -> S2 != NULL ){
                                if( ASTprint_type( p -> declared_type ) == "BOOLEAN" ){
                                  printf( "with type %s = ",  ASTprint_type( p -> declared_type ) );
                                  if( p -> S2 -> value == 1 )
                                    printf( "true" );
                                  else{
                                    printf( "false" );
                                  }
                                } else if( ASTprint_type( p -> declared_type ) == "INT" ){
                                  printf( "with type %s = %d", ASTprint_type( p -> declared_type ), p -> S2 -> value );
                                } 
                              } else{
                                printf( "with type %s", ASTprint_type( p -> declared_type ) );
                              }
                              printf( "\n" );
                              break;
      // Formatting for dealing with array variables
      case A_ARRAYTYPE :      if( p -> S1 -> S1 != NULL ){
                                printf( "[" );
                                if( p -> S1 != NULL )
                                  printf("%s", p -> S1 -> name );
                                else
                                  printf( "%d", p -> S1 -> value );
                                printf( "] " );
                              }
                              break;
      // rules for declaring a method
      case A_METHODDEC :      printf( "METHOD FUNCTION %s %s\n", p -> name, ASTprint_type( p -> declared_type ) );
                              /* print out the parameter list */
                              if( p -> S1 == NULL ) { 
                                  PT( level + 2 ); 
                                  printf ( "(none)" ); 
                                } else{ 
                                  PT( level + 2 );
                                  printf( "( \n" );
                                    ASTprint( level + 2, p -> S1 );
                                  PT( level + 2 );
                                  printf( ") " );
                                }
                              printf( "\n" );
                              ASTprint( level + 2, p -> S2 ); // print out the block
                              break;
      // parameters for methods
      case A_PARAM :          printf( "PARAMETER %s %s offset %d\n", ASTprint_type( p -> declared_type ), p -> name, p -> symbol -> offset );               
                              break;
      // break statement
      case A_BREAK :          printf( "BREAK STATEMENT \n" );
                              break;
      // continue statement
      case A_CONTINUE :       printf( "CONTINUE STATEMENT\n" );
                              break;
      // return statement   
      case A_RETURN :         printf( "RETURN STATEMENT\n" );
                              ASTprint( level, p -> S1 );
                              break;
      // expression handling
      case A_EXPR :           printf( "EXPR " );
                              switch( p -> operator ){
                                case A_PLUS :       printf( "+" );
                                                    break;
                                case A_MINUS :      printf( "-");
                                                    break;
                                case A_DIVIDE :     printf( "/" );
                                                    break;
                                case A_TIMES :      printf( "*" );
                                                    break;
                                case A_MOD :        printf( "%%" );
                                                    break;
                                case A_AND :        printf( "&&" );
                                                    break;
                                case A_OR :         printf( "||" );
                                                    break;
                                case A_LEFTSHIFT :  printf( "<<" );
                                                    break;
                                case A_RIGHTSHIFT : printf( ">>" );
                                                    break;
                                case A_EQ :         printf( "==" );
                                                    break;
                                case A_NEQ :        printf( "!=" );
                                                    break;
                                case A_GT :         printf( ">" );
                                                    break;
                                case A_LT :         printf( "<" );
                                                    break;
                                case A_GEQ :        printf( ">=" );
                                                    break;
                                case A_LEQ :        printf( "<=" );
                                                    break;
                                case A_NOT :        printf( "!" );
                                                    break;      
                                case A_UMINUS:      printf( "-" );
                                                    break;              
                                default : printf("unknown EXPR operator");
                              }
                              printf( "\n" );

                              ASTprint( level + 1, p -> S1 );
                              if( p -> operator != A_NOT )
                                ASTprint( level + 1, p -> S2 );
                              break;
      // if/else statements
      case A_IF :             printf( "IF STATEMENT\n" );
                              ASTprint( level + 1, p -> S1);
                              ASTprint( level + 1, p -> S2);
                              break;
      // branch of if statement to allow else handling
      case A_BODY :           ASTprint(level + 1, p -> S1 );
                              printf( "\n" );
                              if( p -> S2 != NULL ){
                                PT( level );
                                printf( "ELSE\n" );
                                ASTprint( level + 1, p -> S2 );
                              }
                              break;
      // while statements
      case A_WHILE :          printf( "WHILE STATEMENT\n" );
                              ASTprint( level + 1, p -> S1 );
                              ASTprint( level + 1, p -> S2 );
                              break;
      // block handling
      case A_BLOCK :          printf( "BLOCK STATEMENT \n");
                              ASTprint( level + 1, p -> S1 );
                              ASTprint( level + 1, p -> S2 );
                              break;
      // printing number values on their own
      case A_NUMBER :         printf( "NUMBER with value %d\n", p -> value );
                              ASTprint( level + 1, p -> S1 );
                              break;
      // integer constants
      case A_CONSTANT_INT :   printf( "CONSTANT INTEGER %d\n", p -> value );
                              break;
      // boolean constants
      case A_CONSTANT_BOOL :  
                              if( p -> value == 1 )
                                printf( "CONSTANT BOOLEAN true\n" );
                              else
                                printf( "CONSTANT BOOLEAN false\n" );
                              break;
      // string constants
      case A_CONSTANT_STRING : printf( "CONSTANT STRING %s\n", p -> name );
                                break;
      // assignments
      case A_ASSIGN :         printf( "ASSIGNMENT STATEMENT \n" );
                              // PT(level);
                              // printf("%s\n", p -> name);
                              ASTprint( level, p -> S1 );
                              ASTprint( level + 1, p -> S2 );
                              break;
      // variable handling for the left side of an assignment
      case A_VAR_LVALUE:      printf( "Variable %s\n", p -> name );
                              if( p -> S1 != NULL )
                                printf( "[%s]\n", p -> S1 -> value );
                              break;
      // variable handling for the right side of an assignment
      case A_VAR_RVALUE:      printf( "Variable %s", p -> name );
                              if( p -> S1 != NULL ){
                                printf( "\n");
                                PT( level );
                                printf( "[\n");
                                ASTprint(level+1, p -> S1);
                                PT( level );
                                printf( "]");
                              }
                              printf("\n");
                              break;
      // method calling
      case A_METHODCALL :     printf( "METHOD CALL name: %s\n", p -> name );
                              if( p -> S1 == NULL ){
                                PT(level + 1);
                                printf(" (none) ");
                              } else{
                                PT( level + 2 );
                                printf( "{\n" );
                                ASTprint( level + 2, p -> S1 );
                                PT( level + 2 );
                                printf( ")\n" );
                              }
                              break;
      // method arguments
      case A_METHODARG :      printf( "METHOD ARG:\n" );
                              if(p -> S1 != NULL){
                                ASTprint( level + 2, p -> S1 );
                              } else{
                                ASTprint(level + 2, p -> next );
                              }
                              break;      
      // printing method for variable names on their own
      case A_ID :             printf( "Variable %s\n", p -> name );
                              break;
      // if ast can't find an appropriate function, print:
      default: printf( "unknown type in ASTprint\n" );
    } // end switch
    ASTprint( level, p -> next ); // move on to the next function
  } // end else
} // end 


// pre: ptrs to the list of formal and actual parameters
// post: a 1 if all parameters match or 0 if they don't
int check_parameters(ASTnode *formal, ASTnode *actual){
  while(formal != NULL && actual != NULL){
    if(formal -> declared_type != actual -> declared_type){
      return(0);
    }
    formal = formal -> next;
    actual = actual -> next;
    if((formal == NULL && actual != NULL) || (formal != NULL && actual == NULL)){
      return(0);
    }
  }
  return(1);
}
