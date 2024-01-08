%{ 
 /* Alexander Cabos
  * CS370 - lab9.l
  * 12/2/22
  * No changes made for lab 9
  *
  *----------------------------------------------------------------------------------
  * Alexander Cabos
  * CS370 - lab6.l
  * 10/9/22
  * Lab 5 is the base for the compiler, the lex file now contains the HexToInt function that converts hexadecimal 
  * to integers. Tokens have been added for all the supported rules. regular expressions have also been added for 
  * reading comments, characters, strings, integers, variables and hexadecimal numbers. 
  * Linecount has been added too all debug messages.
  * 
  *-----------------------------------------------------------------------------------
  * Alexander Cabos
  * 10/9/22
  * Lab6
  * lab6.l
  *
  * No significant changes made to file during lab 6:
  *   included ast.h
  * 
*/
%}


%{
static int debugsw = 0; /* En/disable debugging messages */\
int linecount = 1; /* counter for line number */
#include "ast.h" 
#include "symtable.h"
#include "y.tab.h"

/* method decl for HexToInt */
int HexToInt(char *s);

/* HexToInt takes a hexadecimal character string and converts it to an integer */
// PRE: hexadecimal input with format "0x"[0-9a-fA-F]
// POST: returns base 10 integer matching the provided hex value
int HexToInt(char *s){
    int i = 0;
    int val = 0;
    
    /* continue loop until at end of string */
    while(*s){
      /* takes ascii value of a character in the string and iterates to the next character with each loop */
      int byte = *s++; // DOUBLE CHECK THIS LATER
      
      /* if the character is between 0 - 9, return the ascii value - '0'. i.e 0 should equal 0 */
      if(byte >= '0' && byte <= '9') 
        byte = byte - '0';

      /* if between 'a' and 'f', should return between 10 - 15 */
      else if(byte >= 'a' && byte <= 'f') 
        byte = byte - 'a' + 10;

      /* if between 'A' and 'F', should also return between 10 - 15 */
      else if(byte >= 'A' && byte <= 'F') 
        byte = byte - 'A' + 10;

      if(i < 2) /* don't adjust the value for the first two characters, since those are "0x" */
        i++;
      else /* shift val 4, add new byte */
        val = (val << 4) | (byte & 0xF);
    }
    /* return integer of hex input */
    return(val);
}


%}


%%

"&&"         {return (T_AND);}
"="          {return (T_ASSIGN);}
"bool"       {return (T_BOOLTYPE);}
"break"      {return (T_BREAK);}
"continue"   {return (T_CONTINUE);}
"."          {return (T_DOT);}
"else"       {return (T_ELSE);}
"=="         {return (T_EQ);}
"extern"     {return (T_EXTERN);}
"false"      {return (T_FALSE);}
"for"        {return (T_FOR);}
"func"       {return (T_FUNC);}
">="         {return (T_GEQ);}
">"          {return (T_GT);}
"if"         {return (T_IF);}
int_lit      {return (T_INTCONSTANT);}
"int"        {return (T_INTTYPE);}
"<<"         {return (T_LEFTSHIFT);}
"<="         {return (T_LEQ);}
"<"          {return (T_LT);}
"!="         {return (T_NEQ);}
"null"       {return (T_NULL);}
"||"         {return (T_OR);}
"package"    {return (T_PACKAGE);}
"return"     {return (T_RETURN);}
">>"         {return (T_RIGHTSHIFT);}
"string"     {return (T_STRINGTYPE);}
"true"       {return (T_TRUE);}
"var"        {return (T_VAR);}
"void"       {return (T_VOID);}
"while"      {return (T_WHILE);}

\n 		                { if(debugsw) fprintf(stderr,"Carriage return %c\n",*yytext); 
                        linecount++; } /* calculate the line when a newline is found */
\'.\'                 { if(debugsw) fprintf(stderr,"Character found, line %d\n", linecount); 
                        yylval.value=yytext[1]; return(T_CHARCONSTANT); } /* return character if a character is found in single quotes */
\".*\"                { if(debugsw) fprintf(stderr,"String found, line %d\n", linecount); 
                        yylval.string=strdup(yytext); return(T_STRINGCONSTANT); } /* return string if string is found in double quotes */
[0-9][0-9]*	          { if(debugsw) fprintf(stderr,"Integer found, line %d\n", linecount); 
                        yylval.value=atoi((const char *)yytext); return(T_INTCONSTANT); } /* read all digit strings as integers */
\/\/.*                { if(debugsw) fprintf(stderr, "Comment found, line %d\n", linecount); 
                        /* this is a comment line for double slash  Return nothing, keep going */ }
[a-zA-Z][a-zA-Z0-9_]* { if(debugsw) fprintf(stderr,"Variable found, line %d\n", linecount); 
                        yylval.string=strdup(yytext); return(T_ID); } /* assign all lowercase letters as variables */
[ \t]		              { if(debugsw) fprintf(stderr,"Whitespace found, line %d\n", linecount); } /* don't do anything for whitespace */
[{},;\-+*/%()!\[\]] 	    { if(debugsw) fprintf(stderr,"Return a token %c, line %d\n", *yytext, linecount); 
                        return (*yytext); } /* read any function tokens */
[0][xX][0-9a-fA-F]+   { if(debugsw) fprintf(stderr, "Hexadecimal Found, line %d\n", linecount);
                        yylval.value=HexToInt(yytext); return (T_INTCONSTANT); } /* take any hex formatted value and convert to an integer */
%%

/* return a 1 to complete when reaching the end of input */
int yywrap(void)
{ return 1;}