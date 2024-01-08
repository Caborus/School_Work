

#	Package foo:		# 

	.section .rodata # start of the RODATA section, strings first

percentD: 	.string "%d" # ALWAYS needed for print int
# NEXT lines are all the strngs in the program
_L0:		.string  "\nPlease enter a number: " 	# global string
_L1:		.string  " : " 	# global string
_L2:		.string  "\n" 	# global string
_L3:		.string  "\n\nPlease enter a number (0 to exit): " 	# global string
_L4:		.string  "\n" 	# global string
_L5:		.string  "Done with the squares!\n\n" 	# global string
_L6:		.string  " squared is " 	# global string
# END of print all the strings in the program

	.data # start of the DATA section for variables


	.comm _SCANFHOLD,8,8  # MANDITORY space for SCANF reads

y:	.long 7  # define a global variable with initial value
	.align 8
	.comm Z,8,8	#  define a global variable
	.align 8
	.comm A,800,800	#  define a global variable
	.align 8
x:	.long 1  # define a global variable with initial value
	.align 8
# END of the DATA section for variables


	.text	# start of CODE segment

	.globl fib
	.type fib, @function
fib:		# Start of Function


	.cfi_startproc			# STANDARD FUNCTION HEADER FOR GAS
	pushq   %rbp			# STANDARD FUNCTION HEADER FOR GAS
	.cfi_def_cfa_offset 16	# STANDARD FUNCTION HEADER FOR GAS
	.cfi_offset 6, -16		# STANDARD FUNCTION HEADER FOR GAS
	movq    %rsp, %rbp		# STANDARD FUNCTION HEADER FOR GAS
	.cfi_def_cfa_register 6	# STANDARD FUNCTION HEADER FOR GAS


	subq $64, %rsp	# carve out activation record for function 
	mov %r8, 0(%rsp)	# copy actual to appropriate slot
	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
	mov %rax, 8(%rsp)	# STORE LHS of expression to memory
	mov $0, %rax	# expression a number
	mov %rax, %rbx	# right hand side needs to be set
	mov 8(%rsp), %rax	# fetch LHS of expression from memory
	cmp %rbx, %rax	# EXPR Greaterthan
	setle %al	# EXPR Lessthan
	mov $1, %rbx	# set rbx to one to filter rax
	and %rbx, %rax	# filter RAX
	cmp $0, %rax	# IF Compare
	JE  _L7	# IF branch to else part
	mov $0, %rax	# expression a number
	leave				# leave the function
	.cfi_def_cfa 7, 8	# STANDRD end function for GAS
	ret

	JMP _L8	# IF S1 end
_L7:		# ELSE target
_L8:		# End of IF

	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
	mov %rax, 16(%rsp)	# STORE LHS of expression to memory
	mov $1, %rax	# expression a number
	mov %rax, %rbx	# right hand side needs to be set
	mov 16(%rsp), %rax	# fetch LHS of expression from memory
	cmp %rbx, %rax	# EXPR Greaterthan
	sete %al	# EXPR Lessthan
	mov $1, %rbx	# set rbx to one to filter rax
	and %rbx, %rax	# filter RAX
	cmp $0, %rax	# IF Compare
	JE  _L9	# IF branch to else part
	mov $1, %rax	# expression a number
	leave				# leave the function
	.cfi_def_cfa 7, 8	# STANDRD end function for GAS
	ret

	JMP _L10	# IF S1 end
_L9:		# ELSE target
_L10:		# End of IF

	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
	mov %rax, 24(%rsp)	# STORE LHS of expression to memory
	mov $1, %rax	# expression a number
	mov %rax, %rbx	# right hand side needs to be set
	mov 24(%rsp), %rax	# fetch LHS of expression from memory
	sub %rbx, %rax	# EXPR SUB
	mov %rax, 32(%rsp)	# Store arg value in our runtime stack
	mov 32(%rsp), %rax	# load calc argument into RAX
	mov %rax, %r8	# store parameter value into appropriate register
		# about to call a function, set up each parameter in the new activation record
	CALL fib	# CALL to function
	mov %rax, 56(%rsp)	# STORE LHS of expression to memory
	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
	mov %rax, 40(%rsp)	# STORE LHS of expression to memory
	mov $2, %rax	# expression a number
	mov %rax, %rbx	# right hand side needs to be set
	mov 40(%rsp), %rax	# fetch LHS of expression from memory
	sub %rbx, %rax	# EXPR SUB
	mov %rax, 48(%rsp)	# Store arg value in our runtime stack
	mov 48(%rsp), %rax	# load calc argument into RAX
	mov %rax, %r8	# store parameter value into appropriate register
		# about to call a function, set up each parameter in the new activation record
	CALL fib	# CALL to function
	mov %rax, %rbx	# right hand side needs to be set
	mov 56(%rsp), %rax	# fetch LHS of expression from memory
	add %rbx, %rax	# EXPR ADD
	leave				# leave the function
	.cfi_def_cfa 7, 8	# STANDRD end function for GAS
	ret

	mov $0, %rax		# Default Return Value
	leave				# leave the function
	.cfi_def_cfa 7, 8	# STANDRD end function for GAS
	ret

	.cfi_endproc			# STANDRD end function for GAS
	.size	fib, .-fib	# STANDRD end function for GAS

	.globl main
	.type main, @function
main:		# Start of Function


	.cfi_startproc			# STANDARD FUNCTION HEADER FOR GAS
	pushq   %rbp			# STANDARD FUNCTION HEADER FOR GAS
	.cfi_def_cfa_offset 16	# STANDARD FUNCTION HEADER FOR GAS
	.cfi_offset 6, -16		# STANDARD FUNCTION HEADER FOR GAS
	movq    %rsp, %rbp		# STANDARD FUNCTION HEADER FOR GAS
	.cfi_def_cfa_register 6	# STANDARD FUNCTION HEADER FOR GAS


	subq $120, %rsp	# carve out activation record for function 
	mov $0, %rax	# expression a number
	mov %rax, 8(%rsp)	# store RHS value in memory
	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov 8(%rsp), %rbx	# Get RHS stored value
	mov %rbx, (%rax)	# ASSIGN final store

	mov  $_L0, %rdi	# RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf		# print a string



	mov $_SCANFHOLD, %rsi	# read in a number
	mov $percentD , %rdi	# rdi has integer format for scanf
	mov $0 , %rax	# No other parameters for scanf
	call  __isoc99_scanf	# call read
	mov _SCANFHOLD, %rax	# bring value on STACK into RAX register for default value

	mov %rax, 24(%rsp)	# store RHS value in memory
	mov $y, %rax	# load gobal variable from data segment
	mov 24(%rsp), %rbx	# Get RHS stored value
	mov %rbx, (%rax)	# ASSIGN final store

_L11:		# WHILE TOP target
	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
	mov %rax, 32(%rsp)	# STORE LHS of expression to memory
	mov $y, %rax	# load global variable from data segment
	mov (%rax), %rax	# expression is identifier
	mov %rax, %rbx	# right hand side needs to be set
	mov 32(%rsp), %rax	# fetch LHS of expression from memory
	cmp %rbx, %rax	# EXPR Greaterthan
	setl %al	# EXPR Lessthan
	mov $1, %rbx	# set rbx to one to filter rax
	and %rbx, %rax	# filter RAX
	cmp $0, %rax	# WHILE compare
	JE  _L12	# WHILE branch out
	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
		# print an INT
	mov %rax, %rsi	# RSI needs the value to print
	mov $percentD, %rdi	# RDI needs to be the print int format
	mov $0, %rax	# RAX needs to be 0 for printf
	call printf	# print a number from expression


	mov  $_L1, %rdi	# RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf		# print a string


	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
	mov %rax, 56(%rsp)	# Store arg value in our runtime stack
	mov 56(%rsp), %rax	# load calc argument into RAX
	mov %rax, %r8	# store parameter value into appropriate register
		# about to call a function, set up each parameter in the new activation record
	CALL fib	# CALL to function
		# print an INT
	mov %rax, %rsi	# RSI needs the value to print
	mov $percentD, %rdi	# RDI needs to be the print int format
	mov $0, %rax	# RAX needs to be 0 for printf
	call printf	# print a number from expression


	mov  $_L2, %rdi	# RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf		# print a string


	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
	mov %rax, 80(%rsp)	# STORE LHS of expression to memory
	mov $1, %rax	# expression a number
	mov %rax, %rbx	# right hand side needs to be set
	mov 80(%rsp), %rax	# fetch LHS of expression from memory
	add %rbx, %rax	# EXPR ADD
	mov %rax, 88(%rsp)	# store RHS value in memory
	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov 88(%rsp), %rbx	# Get RHS stored value
	mov %rbx, (%rax)	# ASSIGN final store

	JMP _L11	# WHILE jump back
_L12:		# End of WHILE

_L13:		# WHILE TOP target
	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
	mov %rax, 40(%rsp)	# STORE LHS of expression to memory
	mov $0, %rax	# expression a number
	mov %rax, %rbx	# right hand side needs to be set
	mov 40(%rsp), %rax	# fetch LHS of expression from memory
	cmp %rbx, %rax	# EXPR Greaterthan
	setne %al	# EXPR Lessthan
	mov $1, %rbx	# set rbx to one to filter rax
	and %rbx, %rax	# filter RAX
	cmp $0, %rax	# WHILE compare
	JE  _L14	# WHILE branch out
	mov  $_L3, %rdi	# RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf		# print a string



	mov $_SCANFHOLD, %rsi	# read in a number
	mov $percentD , %rdi	# rdi has integer format for scanf
	mov $0 , %rax	# No other parameters for scanf
	call  __isoc99_scanf	# call read
	mov _SCANFHOLD, %rax	# bring value on STACK into RAX register for default value

	mov %rax, 56(%rsp)	# store RHS value in memory
	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov 56(%rsp), %rbx	# Get RHS stored value
	mov %rbx, (%rax)	# ASSIGN final store

	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
	mov %rax, 64(%rsp)	# STORE LHS of expression to memory
	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
	mov %rax, %rbx	# right hand side needs to be set
	mov 64(%rsp), %rax	# fetch LHS of expression from memory
	imul %rbx, %rax	# EXPR MUL
	mov %rax, 72(%rsp)	# store RHS value in memory
	mov $y, %rax	# load gobal variable from data segment
	mov 72(%rsp), %rbx	# Get RHS stored value
	mov %rbx, (%rax)	# ASSIGN final store

	mov  $_L4, %rdi	# RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf		# print a string


	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
	mov %rax, 88(%rsp)	# STORE LHS of expression to memory
	mov $0, %rax	# expression a number
	mov %rax, %rbx	# right hand side needs to be set
	mov 88(%rsp), %rax	# fetch LHS of expression from memory
	cmp %rbx, %rax	# EXPR Greaterthan
	sete %al	# EXPR Lessthan
	mov $1, %rbx	# set rbx to one to filter rax
	and %rbx, %rax	# filter RAX
	cmp $0, %rax	# IF Compare
	JE  _L15	# IF branch to else part
	mov  $_L5, %rdi	# RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf		# print a string


	JMP _L14	# BREAK statement
	JMP _L16	# IF S1 end
_L15:		# ELSE target
_L16:		# End of IF

	mov  $0, %rax	# get Identifier offset
	add %rsp, %rax	# we have direct reference to memory
	mov (%rax), %rax	# expression is identifier
		# print an INT
	mov %rax, %rsi	# RSI needs the value to print
	mov $percentD, %rdi	# RDI needs to be the print int format
	mov $0, %rax	# RAX needs to be 0 for printf
	call printf	# print a number from expression


	mov  $_L6, %rdi	# RDI is the label address
	mov $0, %rax	# RAX need to be zero
	call printf		# print a string


	mov $y, %rax	# load global variable from data segment
	mov (%rax), %rax	# expression is identifier
		# print an INT
	mov %rax, %rsi	# RSI needs the value to print
	mov $percentD, %rdi	# RDI needs to be the print int format
	mov $0, %rax	# RAX needs to be 0 for printf
	call printf	# print a number from expression


	JMP _L13	# CONTINUE statement
	JMP _L13	# WHILE jump back
_L14:		# End of WHILE

	mov $0, %rax		# Default Return Value
	leave				# leave the function
	.cfi_def_cfa 7, 8	# STANDRD end function for GAS
	ret

	.cfi_endproc			# STANDRD end function for GAS
	.size	main, .-main	# STANDRD end function for GAS

	.ident  "GCC: (SUSE Linux) 7.5.0"
	.section        .note.GNU-stack,"",@progbits
