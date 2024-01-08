

//  Subprograms which output MIPS code.
//
//   Shaun Cooper Spring 2015
/// Updated Spring 2017 to utilize the data segement portion of MIPS instead of advancing the Stack and Global pointers
/// to accomodate global variables.
//  Shaun Cooper December 2020 updated for DECAF format

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"
#include "emit.h"

int LTEMP = 0; // current count for the number of labels

char  * CURRENT_FUNC; //need to know Current Function for return on main

// structure for a list of labels for while loops, allows for nesting loops and use of break and continue
struct LabelList{
   char * Label;
   struct LabelList *next;
}; // end LabelList

// GOBACK is for returning to the start of the loop, GETOUT breaks out of the loop
struct LabelList *GOBACK, *GETOUT;
   
// PRE: The name of the label to be inserted and the list the label should be added to
// POST: Label gets added to the stack for use in while loops
struct LabelList *PushLabel(char *L, struct LabelList *target){
      struct LabelList *p;
      p = (struct LabelList*)(malloc(sizeof(PushLabel)));
      p->Label = L;
      p->next = target;
      return p;
} // end PushLabel

// PRE: None
// POST: Remove the top label from the GOBACK stack
void PopContinue(){
   GOBACK = GOBACK->next;
} // end PopContinue()


// PRE: None
// POST: Remove the top label from the GETOUT stack
void PopBreak(){
   GETOUT = GETOUT->next;
} // end PopBreak()


// PRE: None
// Post: Returns a character string for a label with format "_L%d", where %d is LTEMP
char * CREATE_LABEL(){
  char s[100];
  sprintf(s, "_L%d", LTEMP);
  LTEMP++;
  return(strdup(s));
} // end CREATE_LABEL()


// helper function for pretty printing
// pre: label, action, comment
// post: clean gas code in file
void emit(FILE * fp, char * label, char * action, char * comment){
   if( strcmp(label, "") == 0 ){ // no label
      if( strcmp(action, "") == 0 ){ // no label, no action
         if( strcmp(comment, "") == 0 ){ // no label, no action, no comment
            fprintf(fp, "\n");
         } else{ // no label, no action, yes comment
            fprintf(fp, "\t\t# %s\n", comment);
         }
      } else{ // no label, yes action
         if( strcmp(comment, "") == 0 ){ // no label, yes action, no comment
            fprintf(fp, "\t%s\n", action);
         } else{ // no label, yes action, yes comment
            fprintf(fp, "\t%s\t# %s\n", action, comment);
         }

      }
   } else{ // yes label
      if( strcmp(action, "") ){ // yes label, no action
         if( strcmp(comment, "") == 0 ){ // yes label, no action, no comment
            fprintf(fp, "%s:\n", label);
         } else{ // yes label, no action, yes comment
            fprintf(fp, "%s:\t\t# %s\n", label, comment);
         }
      } else{ // yes label, yes action
         if( strcmp(comment, "") == 0 ){ // yes label, yes action, no comment
            fprintf(fp, "%s:\t%s\n", label, action);
         } else{ // yes label, yes action, yes comment
            fprintf(fp, "%s:\t%s\t# %s\n", label, action, comment);
         }
      }
   } // end if/else for label

} // end emit


// pre: ptr to ast tree and opened file
// post: gas code in fp that implements asttree
void EMIT( ASTnode * p, FILE *fp ){
   char s[100];
   
   sprintf(s, "\n\n#\tPackage %s", p -> S2 -> name );
   emit(fp, s, "", "\n" );
   fprintf(fp, "\t.section .rodata # start of the RODATA section, strings first\n\n");
   
   fprintf(fp, "percentD: \t.string \"%%d\" # ALWAYS needed for print int\n");
   fprintf(fp, "# NEXT lines are all the strngs in the program\n");
   
   Emit_Strings(p, fp);
   
   fprintf(fp, "# END of print all the strings in the program\n\n");
   fprintf(fp, "\t.data # start of the DATA section for variables\n\n\n");
   fprintf(fp, "\t.comm _SCANFHOLD,%d,%d  # MANDITORY space for SCANF reads\n\n", WSIZE, WSIZE);
   
   Emit_Globals(p, fp);

   fprintf(fp, "# END of the DATA section for variables\n\n\n");
   fprintf(fp, "\t.text\t# start of CODE segment\n\n");

   Emit_AST(p, fp);

 	fprintf(fp, "\t.ident  \"GCC: (SUSE Linux) 7.5.0\"\n");
	fprintf(fp, "\t.section        .note.GNU-stack,\"\",@progbits\n");

} // end EMIT()


// pre: ptr to ast tree and open file
// post: output to fp that has label strings pairs
void Emit_Strings(ASTnode *p, FILE *fp){
   if(p == NULL) return; 

   char s[100]; 
   
   // if this node is a string, create a new label and create gas code assigning the 
   //    string to that label.
   if(p -> type == A_CONSTANT_STRING){ 
     
      p -> label = CREATE_LABEL();

      fprintf(fp, "%s:\t\t.string  %s \t# global string\n", p -> label, p -> name );
       
   } // end if
   
   // traverse the whole AST looking for strings
   Emit_Strings(p->S1, fp);
   Emit_Strings(p->S2, fp);
   Emit_Strings(p->next, fp);
   return;
} // end Emit_Strings


// pre: ptr to ast node that points to return or null decl and open file
// post: gas code for the return, %rax is 0 for null and set by emit_expr for expr
void Emit_Return(ASTnode * p, FILE *fp){
   char s[100];
   // if the pointer is null, default return, else return the expression
   if(p == NULL || p -> S1 == NULL) 
      emit(fp,"", "mov $0, %rax\t", "Default Return Value");
   else
      Emit_Expr(p->S1, fp);

   emit(fp, "", "leave\t\t\t", "leave the function");
   emit(fp, "", ".cfi_def_cfa 7, 8", "STANDRD end function for GAS");
   emit(fp, "", "ret\n","");
	
} // end Emit_Return()


// PRE: PTR to ast node that points to an expression and open file
// POST: gas code for that expression
void Emit_Expr(ASTnode *p, FILE *fp){
   char s[100];

   switch (p->type){
      case A_CONSTANT_INT :
                           sprintf(s, "mov $%d, %%rax", p->value);
                           emit(fp, "", s, "expression a number");
                           return;
                           break;
      case A_CONSTANT_BOOL :
                           sprintf(s, "mov  $%d,  %%rax", p->value);
                           emit(fp, "", s, "expresion a number");
                           break;
      case A_METHODCALL :
                           Emit_Method_Call(p, fp);
                           break;
      case A_EXPR :
                           if(p->S1 != NULL){
                              Emit_Expr(p->S1, fp);
                              sprintf(s, "mov %%rax, %d(%%rsp)", p->symbol->offset*WSIZE);
                              emit(fp, "", s, "STORE LHS of expression to memory");
                           if(p->S2 != NULL){
                              Emit_Expr(p->S2, fp);
                              emit(fp, "", "mov %rax, %rbx", "right hand side needs to be set");
                              sprintf(s, "mov %d(%%rsp), %%rax", p->symbol->offset*WSIZE);
                              emit(fp, "", s, "fetch LHS of expression from memory");
                           }
                              Emit_Operator(p, fp);
                           }
                           break;
      case A_VAR_RVALUE :      
                           if(p->S1 != NULL){
                              sprintf(s, "mov  $%d,  %%rax", p->S1->value);
                              emit(fp, "", s, "expression a number");
                              emit(fp, "", "mov %rax, %rbx", "array component");
                              sprintf(s, "shl $%d, %%rbx", LOG_WSIZE); 
                              emit(fp, "", s, "multiply wordsize for array reference");
                           }
                           sprintf(s, "mov $%s, %%rax", p->symbol->name);
                           if(p->symbol->level == 0)
                              emit(fp, "", s, "load global variable from data segment");
                           else{
                              sprintf(s, "mov  $%d, %%rax", p->symbol->offset*WSIZE);
                              emit(fp, "", s, "get Identifier offset");
                              emit(fp, "", "add %rsp, %rax", "we have direct reference to memory"); 
                           }
	                        if(p->S1 != NULL)
                              emit(fp, "", "add %rbx, %rax", "move add on %rbx as this is an array reference");
                           emit(fp, "", "mov (%rax), %rax", "expression is identifier");
                           break;
      default :
               break;
   }
}


// PRE: PTR to an expression and open file
// POST: gas code for the operator within that expression
void Emit_Operator(ASTnode *p, FILE *fp){
   char s[100];
   switch( p -> operator ){
      case A_PLUS :       
                           emit(fp, "", "add %rbx, %rax", "EXPR ADD");
                           break;
      case A_MINUS :      
                           emit(fp, "", "sub %rbx, %rax", "EXPR SUB");
                           break;
      case A_DIVIDE :     
                           emit(fp, "", "mov $0, %rdx", "EXPR upper word 0 DIV");
	                        emit(fp, "", "idiv %rbx", "EXPR DIVIDE");
                           break;
      case A_TIMES :      
                           emit(fp, "", "imul %rbx, %rax", "EXPR MUL");
                           break;
      case A_MOD :        
                           emit(fp, "", "mov $0, %rdx", "EXPR upper word 0 DIV");
	                        emit(fp, "", "idiv %rbx", "EXPR MOD");
                           emit(fp, "", "mov %rdx, %rax", "MOD needs to be in RAX");
                           break;
      case A_AND :        
                           emit(fp, "", "and %rbx, %rax", "EXPR AND");
                           break;
      case A_OR :         
                           emit(fp, "", "or %rbx, %rax", "EXPR OR");
                           break;
      case A_LEFTSHIFT :   break; // Dr. Cooper said I don't have to
      case A_RIGHTSHIFT :  break; // Dr. Cooper said I don't have to
      case A_EQ :         
                           emit(fp, "", "cmp %rbx, %rax", "EXPR Greaterthan");
                           emit(fp, "", "sete %al", "EXPR Lessthan");
                           emit(fp, "", "mov $1, %rbx", "set rbx to one to filter rax");
                           emit(fp, "", "and %rbx, %rax", "filter RAX");
                           break;
      case A_NEQ :        
                           emit(fp, "", "cmp %rbx, %rax", "EXPR Greaterthan");
                           emit(fp, "", "setne %al", "EXPR Lessthan");
                           emit(fp, "", "mov $1, %rbx", "set rbx to one to filter rax");
                           emit(fp, "", "and %rbx, %rax", "filter RAX");
                           break;
      case A_GT :         
                           emit(fp, "", "cmp %rbx, %rax", "EXPR Greaterthan");
                           emit(fp, "", "setg %al", "EXPR Lessthan");
                           emit(fp, "", "mov $1, %rbx", "set rbx to one to filter rax");
                           emit(fp, "", "and %rbx, %rax", "filter RAX");
                           break;
      case A_LT :         
                           emit(fp, "", "cmp %rbx, %rax", "EXPR Greaterthan");
                           emit(fp, "", "setl %al", "EXPR Lessthan");
                           emit(fp, "", "mov $1, %rbx", "set rbx to one to filter rax");
                           emit(fp, "", "and %rbx, %rax", "filter RAX");
                           break;
      case A_GEQ :        
                           emit(fp, "", "cmp %rbx, %rax", "EXPR Greaterthan");
                           emit(fp, "", "setge %al", "EXPR Lessthan");
                           emit(fp, "", "mov $1, %rbx", "set rbx to one to filter rax");
                           emit(fp, "", "and %rbx, %rax", "filter RAX");
                           break;
      case A_LEQ :        
                           emit(fp, "", "cmp %rbx, %rax", "EXPR Greaterthan");
                           emit(fp, "", "setle %al", "EXPR Lessthan");
                           emit(fp, "", "mov $1, %rbx", "set rbx to one to filter rax");
                           emit(fp, "", "and %rbx, %rax", "filter RAX");
                           break;
      case A_NOT :        
                           emit(fp, "", "not %rax", "EXPR NOT");
                           emit(fp, "", "mov $1, %rbx", "set rbx to one to filter rax");
                           emit(fp, "", "and %rbx, %rax", "filter RAX");
                           break;      
      case A_UMINUS:      
                           
                           emit(fp, "", "neg %rax", "EXPR UMINUS");
                           return;
                           break;              
      default : // printf("unknown EXPR operator");
                           break;
   }
}


// pre: ptr to ast tree and open file
// post: gas code for global variables to be placed in .data section
void Emit_Globals(ASTnode *p, FILE *fp){
   if( p == NULL ) return;
   
   char s[100];

   // if a global variable is found, create gas code to represent that variable
   if((p -> type == A_VARDEC) && (p -> symbol -> level == 0)){
      if(p -> S2 != NULL){ // this is for if the variable has an initial value
         fprintf(fp, "%s:\t.long %d  # define a global variable with initial value\n", p -> name, p -> S2 -> value);
      } else{ // no initial value
         fprintf(fp, "\t.comm %s,%d,%d	#  define a global variable\n", p -> name, (p -> symbol -> mysize * WSIZE), (p -> symbol -> mysize * WSIZE));
      }
      fprintf(fp, "\t.align %d\n", WSIZE);
   }

   // traverse entire tree
   Emit_Globals(p -> S1, fp);
   Emit_Globals(p -> S2, fp);
   Emit_Globals(p -> next, fp);
} // end Emit_Globals()


// PRE: ptr to a method declaration and open file
// POST: gas code for declaring that method
void Emit_Method_Decl(ASTnode *p, FILE *fp){
   char s[100];
   
   sprintf(s, ".globl %s", p -> name);
 	emit(fp, "", s, "");
   sprintf(s, ".type %s, @function", p -> name);
	emit(fp, "", s, "" );
   emit(fp, p->name, "", "Start of Function\n\n");
	emit(fp, "", ".cfi_startproc\t\t", "STANDARD FUNCTION HEADER FOR GAS");
	emit(fp, "", "pushq   %rbp\t\t", "STANDARD FUNCTION HEADER FOR GAS");
	emit(fp, "", ".cfi_def_cfa_offset 16", "STANDARD FUNCTION HEADER FOR GAS");
	emit(fp, "", ".cfi_offset 6, -16\t", "STANDARD FUNCTION HEADER FOR GAS");
	emit(fp, "", "movq    %rsp, %rbp\t", "STANDARD FUNCTION HEADER FOR GAS");
	emit(fp, "", ".cfi_def_cfa_register 6", "STANDARD FUNCTION HEADER FOR GAS\n\n");
   sprintf(s, "subq $%d, %%rsp", p->symbol->mysize*WSIZE);
   emit(fp,"",s,"carve out activation record for function ");

   // get the parameters for the method
   ASTnode *parms = p -> symbol -> fparms;
   int i = 8;

   // print gas code assigning each parameter to an appropriate register (r8-r15)
   while(parms != NULL){
      sprintf(s, "mov %%r%d, %d(%%rsp)", i, parms -> symbol -> offset * WSIZE);
      emit(fp, "", s, "copy actual to appropriate slot");
      i++;
      parms = parms -> next;
      if(i > 15){ // break if we pass r15
         printf("TOO MANY PARAMETERS");
         break;
      } // end if
   } // end while
   
   Emit_AST(p -> S2, fp);

   // implied return
   Emit_Return(NULL, fp);

   emit(fp, "", ".cfi_endproc\t\t", "STANDRD end function for GAS");
   sprintf(s, ".size	%s, .-%s", p->name, p->name);
   emit(fp, "", s, "STANDRD end function for GAS\n");

} // end Emit_Method_Decl()


// PRE: ptr to a method call and an open file
// POST: gas code for calling back to that method
void Emit_Method_Call(ASTnode *p, FILE *fp){
   char s[100];

   if(p == NULL){
      // printf("ERROR EMIT METHOD CALL\n");
      return;
   } // end if

   // if it's an extern, take care of those separately
   if(p->symbol->SubType == ID_Sub_Type_Extern){
      Emit_Extern_Method_Call(p, fp);
      return;
   } // end if

   // get the parameters for the method call
   ASTnode *parms = p -> S1;

   // go through all parameters, and create gas code storing each value in the runtime stack
   while(parms != NULL){
      Emit_Expr(parms->S1, fp);

      sprintf(s, "mov %%rax, %d(%%rsp)", parms -> symbol -> offset * WSIZE);
      emit(fp, "", s, "Store arg value in our runtime stack");

      parms = parms -> next;
   } // end while

   // reset back to first parameter
   parms = p -> S1;
   int i = 8;

   // gas code to move the parameters into the registers
   while(parms != NULL){
      sprintf(s, "mov %d(%%rsp), %%rax", parms -> symbol -> offset * WSIZE );
      emit(fp, "", s, "load calc argument into RAX");

      sprintf(s, "mov %%rax, %%r%d", i++);
      emit(fp, "", s, "store parameter value into appropriate register");
      parms = parms -> next;

      if(i > 15){ // break if over r15
         printf("TOO MANY PARAMETERS");
         break;
      }
   }

   // gas code to call the function after all registers are set
   emit(fp, "", "", "about to call a function, set up each parameter in the new activation record");
   sprintf(s, "CALL %s", p->name);
   emit(fp, "", s, "CALL to function");
} // end method call


// PRE: ptr to a call for an external method and an open file
// POST: gas code for performing the fuction of that method (print_string, print_int, or read_int)
void Emit_Extern_Method_Call(ASTnode *p, FILE *fp){
   char s[100];

   // gas code for printing a string
   if(strcmp("print_string", p->name) == 0){ 
      sprintf(s, "mov  $%s, %%rdi",	p->S1->S1->label);
      emit(fp, "", s, "RDI is the label address");
	   emit(fp, "", "mov $0, %rax", "RAX need to be zero");
	   emit(fp, "", "call printf\t", "print a string\n\n");
      return;
   } // end if

   // gas code for printing an integer
   if(strcmp("print_int", p->name) == 0){
      Emit_Expr(p->S1->S1, fp);
      emit(fp, "", "", "print an INT");
      emit(fp, "", "mov %rax, %rsi", "RSI needs the value to print");
      emit(fp, "", "mov $percentD, %rdi", "RDI needs to be the print int format");
      emit(fp, "", "mov $0, %rax", "RAX needs to be 0 for printf");
      emit(fp, "", "call printf", "print a number from expression\n\n");
      return;
   } // end if

   // gas code for reading a user input integer from terminal
   if(strcmp("read_int", p->name) == 0){
      fprintf(fp, "\n");
      emit(fp, "", "mov $_SCANFHOLD, %rsi", "read in a number"); 
      emit(fp, "", "mov $percentD , %rdi", "rdi has integer format for scanf");
      emit(fp, "", "mov $0 , %rax", "No other parameters for scanf");
      emit(fp, "", "call  __isoc99_scanf", "call read");
      emit(fp, "", "mov _SCANFHOLD, %rax", "bring value on STACK into RAX register for default value\n"); 
      return;
   } // end if

   fprintf(stderr,"Extern method %s is unknown\n", p->name);
   return; 
} // end Emit_Extern_Method_Call()


// PRE: ptr to an assignment statement and an open file
// POST: gas code to assign a value with the results of the rhs expression
void Emit_Assign(ASTnode *p, FILE *fp){
   char s[100];

   if((p->S1 != NULL) && (p->S2 != NULL)){
      Emit_Expr(p->S2, fp);
      sprintf(s, "mov %%rax, %d(%%rsp)", p->symbol->offset*WSIZE);
      emit(fp, "", s, "store RHS value in memory");
      if(p->S1->S1 != NULL){
         Emit_Expr(p->S1->S1, fp);
         emit(fp, "", "mov %rax, %rbx", "array component");
         sprintf(s, "shl $%d, %%rbx", LOG_WSIZE); 
         emit(fp, "", s, "multiply wordsize for array reference");
      } // end if
      sprintf(s, "mov $%s, %%rax", p->S1->name);
      if(p->S1->symbol->level == 0)
         emit(fp, "", s, "load gobal variable from data segment");
      else{
         sprintf(s, "mov  $%d, %%rax", p->S1->symbol->offset*WSIZE);
         emit(fp, "", s, "get Identifier offset");
         emit(fp, "", "add %rsp, %rax", "we have direct reference to memory"); 
      } // end if/else

      if(p->S1->S1 != NULL)
         emit(fp, "", "add %rbx, %rax", "move add on %rbx as this is an array reference");
      sprintf(s, "mov %d(%%rsp), %%rbx", p->symbol->offset*WSIZE);
      emit(fp, "", s, "Get RHS stored value");
      emit(fp, "", "mov %rbx, (%rax)", "ASSIGN final store\n"); 
   } // end if
   return;
} // end Emit_Assign()


// PRE: ptr to an if/else statement and an open file
// POST: gas code for implementing an if/else condition
void Emit_If(ASTnode *p, FILE *fp){
   char *ifjmp;
   char *eljmp;
   char s[100];
   
   eljmp = CREATE_LABEL();
   ifjmp = CREATE_LABEL();

   Emit_Expr(p->S1, fp);

   emit(fp, "", "cmp $0, %rax", "IF Compare");
   sprintf(s, "JE  %s", eljmp);

   emit(fp, "", s, "IF branch to else part");
   if(p->S2->S1 != NULL) Emit_AST(p->S2->S1, fp);
   sprintf(s, "JMP %s", ifjmp);
   emit(fp, "", s, "IF S1 end");
   emit(fp, eljmp, "", "ELSE target");
   if(p->S2->S2 != NULL) Emit_AST(p->S2->S2, fp);
   emit(fp, ifjmp, "", "End of IF\n");
   
}


// PRE: ptr to a while loop statement and an open file
// POST: gas code for creating a while loop 
void Emit_While(ASTnode *p, FILE *fp){
   char s[100];

   // create labels for looping and breaking out
   GOBACK = PushLabel(CREATE_LABEL(), GOBACK);
   GETOUT = PushLabel(CREATE_LABEL(), GETOUT);


   emit(fp, GOBACK->Label, "", "WHILE TOP target");
   
   Emit_Expr(p->S1, fp);
   
	emit(fp, "", "cmp $0, %rax", "WHILE compare");
   sprintf(s, "JE  %s", GETOUT->Label);
	emit(fp, "", s, "WHILE branch out");
   Emit_AST(p->S2, fp);
   sprintf(s, "JMP %s", GOBACK->Label);
   emit(fp, "", s, "WHILE jump back");
   emit(fp, GETOUT->Label, "", "End of WHILE\n");

   // remove labels for this while loop
   PopBreak();
   PopContinue();
} // end Emit_While()


// pre: ptr to ast and open file
// post: gas code that generates all the gas elements for
//    the rest of the tree other than global vars and strings
void Emit_AST(ASTnode *p, FILE *fp){
   char s[100];

   if( p == NULL ) return;

   switch (p->type) {
      case A_PROGRAM :  
                           Emit_AST(p->S1,fp);
			                  Emit_AST(p->S2,fp);
                           break;

      case A_PACKAGE :  
                           Emit_AST(p->S1,fp);
			                  Emit_AST(p->S2,fp);
                           break;

		case A_METHODDEC : 
                           Emit_Method_Decl(p, fp);
                           break;

      case A_BREAK : 
                           // gas code for breaking out of a while loop
                           sprintf(s, "JMP %s", GETOUT->Label);
                           emit(fp, "", s, "BREAK statement");
                           break;

      case A_CONTINUE : 
                           // gas code for jumping back to the beginning of a while loop
                           sprintf(s, "JMP %s", GOBACK->Label);
                           emit(fp, "", s, "CONTINUE statement");
                           break;

      case A_RETURN : 
                           Emit_Return(p, fp); 
                           break;

      case A_IF :  
                           Emit_If(p, fp);
                           break;

      case A_WHILE : 
                           Emit_While(p, fp);
                           break;

      case A_BLOCK : 
                           Emit_AST(p->S2, fp);
                           break;

      case A_ASSIGN : 
                           Emit_Assign(p, fp);
                           break;

      case A_METHODCALL :
                           Emit_Method_Call(p, fp);
                           break;

      case A_METHODARG :         break;
      case A_ID :                break;
      case A_EXTERN :            break;
      case A_EXTERN_TYPE :       break;
		case A_VARDEC :            break;
      case A_PARAM :             break;
      case A_EXPR :              break;
      case A_ARRAYTYPE :         break;
      case A_NUMBER :            break;
      case A_CONSTANT_INT :      break;
      case A_CONSTANT_BOOL :     break;
      case A_CONSTANT_STRING :   break;
      case A_BODY :              break;
      case A_VAR_LVALUE:         break;
      case A_VAR_RVALUE:         break;

	   default: printf("\n\n%d:  unknown type in EMITAST\n\n",p->type);

   }
   Emit_AST(p->next,fp); // always do the next
} // end Emit_AST()
