;partners: asr9, jrvasko2
;
;My implementation follows the algorithm given on the wiki very closely. I first read the user's input and echo it 
;to the screen using GETC and OUT. After this I call evaluate, where it determines if the input is equals, a space,
;an operator, or a number. If it is equals we perform the necessary checks (printing an error message if the checks
;fail) and print the hex value of the desired output, if it is space we return, if it is a number we subtract
;ASCII 0 from the value and push, and if it is an operator we call the necessary subroutine (or print error message
;if operator is invalid). The program halts after EQUALS and ERROR_MESSAGE, otherwise it will return and read the
;next character

.ORIG x3000
	
GETC				;get character in R0
OUT				;print character to screen
JSR EVALUATE			;call evaluate to determine what to do with data in R0
BRnzp #-4			;loop back so we can capture next character
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R3- value to print in hexadecimal
PRINT_HEX
ST R7, PRINT_SaveR7		;store R7 so OUT doesn't cause infinite loop
ST R5, PRINT_SaveR5		;store R5 so it doesn't remain a loop counter when we return

AND R4,R4,#0		; clear R4
	ADD R4,R4,#4		; set R4 to 4, the number of sets of 4 bits we need to analyze
OUTER_LOOP
	AND R0,R0,#0		; clear R0
	AND R5,R5,#0		; clear R5
	ADD R5,R5,#3		; set R5 to 4, the number of bit shifts we need to do for each set of 4 bits
INNER_LOOP
	ADD R3,R3,#0		; for branch
	BRzp POSITIVE_VALUE	; branch if MSB is not 1
	ADD R0,R0,#1		; increment R0 if we find the MSB of R3 is 1
POSITIVE_VALUE
	ADD R3,R3,R3		; left shift R3
	ADD R5,R5,xFFFF		; decrement R5
	BRn SKIP_SHIFT		; branch if 3 loops have been completed (we left shift R3 4 times, R0 3 times)
	ADD R0,R0,R0		; left shift R0 (holds data we will print out)
SKIP_SHIFT
	ADD R5,R5,#0		; for branch
	BRzp INNER_LOOP		; if R5 isn't negative, we haven't completed the 4 loops we need yet
	AND R6,R6,#0		; clear R6
	ADD R6,R6,#9		; set R6 to 9 (which we will use to differentiate between letter and number)
	NOT R6,R6
	ADD R6,R6,#1		; negate R6
	ADD R6,R0,R6		; add R0 to R6 and store the result in R0
	BRp LETTER		; branch if the result is positive (which indicates that we need to print a letter)
	LD R6,NUMBER_OFFSET	; load number offset into R6
	BRnzp PRINT_NUM		; branch to avoid loading letter offset into R6
LETTER	
	LD R6,LETTER_OFFSET	; load letter offset into R6
PRINT_NUM
	ADD R0,R0,R6		; get correct ascii code into R0
	OUT
	ADD R4,R4,xFFFF		; decrement R4
	BRp OUTER_LOOP		; if we haven't completed 4 outer loops yet, repeat
	LD R7, PRINT_SaveR7	; load correct value for R7 back in
	LD R5, PRINT_SaveR5
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard
;R1 - temporary register
;R2 - temporary register
;R6 - current numerical output
;
;
EVALUATE
	LD R1,EQUALS_ASCII	;check if value in R0 matches ASCII code of equals
	ADD R1,R0,R1
	BRz EQUALS
	LD R1,SPACE_ASCII	;check if value in R0 matches ASCII code of space
	ADD R1,R0,R1
	BRz SPACE
	LD R1,ZERO_ASCII	;check if value in R0 is less than 0
	ADD R1,R0,R1
	BRn OPERATOR		;if less than 0, value must be operator or invalid
	LD R1,NINE_ASCII
	ADD R1,R0,R1
	BRp OPERATOR		;if greater than 9, value must be operator or invalid
	LD R1,ZERO_ASCII
	ADD R0,R0,R1		;subtract ascii 0 from R0 (for push)
	ST R7,EVAL_SaveR7	
	JSR PUSH
	LD R7,EVAL_SaveR7
	RET

EQUALS
	LD R1,STACK_TOP		;load hex value of stack top into R1
	NOT R1,R1;
	ADD R1,R1,#1		;invert R1
	LD R2,STACK_START	;load starting value of stack top into R2
	ADD R1,R2,R1		;add inverted stack top and stack start to get number of stack elements
	BRnz ERROR_MESSAGE	;if result of this operation is 0, stack had 0 elements on it
	ADD R1,R1,xFFFF		;decrement R1
	BRp ERROR_MESSAGE	;if result of this operation is >0, stack had >1 elements on it
	JSR POP			;if we've passed all the cases, we just need to pop and print result
	ADD R5,R0,#0		;store value in R0 in R5	
	ADD R3,R0,#0		;store value to print in R3 for hex print
	JSR PRINT_HEX
	HALT
SPACE
	RET			;space just keeps looping through characters
	
ERROR_MESSAGE
	LEA R0, ERROR_STRING	;load address of error string into R0
	PUTS			;print error message to screen
	HALT

OPERATOR
	LD R1,STACK_TOP		;load hex value of stack top into R1
	NOT R1,R1;
	ADD R1,R1,#1		;invert R1
	LD R2,STACK_START	;load starting value of stack top into R2
	ADD R1,R2,R1		;add inverted stack top and stack start to get number of stack elements
	ADD R1,R1,xFFFF		;decrement R1 for the following check
	BRnz ERROR_MESSAGE	;if result of this operation is not positive, stack had 1 or less elements
	
	LD R1,MULT_ASCII
	ADD R1,R0,R1
	BRnp #9			;if R0 is not mult operand, branch past our multiplication code
	ST R7, EVAL_SaveR7	;store R7, because we are about to call nested subroutine
	JSR POP
	ADD R3,R0,#0		;store result of pop into R3
	JSR POP
	ADD R4,R0,#0		;store result of pop into R4
	JSR MUL
	JSR PUSH
	LD R7, EVAL_SaveR7
	RET
	
	LD R1,PLUS_ASCII
	ADD R1,R0,R1
	BRnp #9			;if R0 is not adding operand, branch past the adding code
	ST R7, EVAL_SaveR7
	JSR POP
	ADD R3,R0,#0		;store result of pop into R3
	JSR POP
	ADD R4,R0,#0		;store result of pop into R4
	JSR PLUS
	JSR PUSH
	LD R7, EVAL_SaveR7
	RET
	
	LD R1,MINUS_ASCII
	ADD R1,R0,R1
	BRnp #9			;if R0 is not minus operand, branch past the minus code
	ST R7, EVAL_SaveR7
	JSR POP
	ADD R3,R0,#0		;store result of pop into R3
	JSR POP
	ADD R4,R0,#0		;store result of pop into R4
	JSR MIN
	JSR PUSH
	LD R7, EVAL_SaveR7
	RET

	LD R1,DIVIDE_ASCII
	ADD R1,R0,R1
	BRnp #9			;if R0 is not divide operand, branch past the divide code
	ST R7, EVAL_SaveR7
	JSR POP
	ADD R3,R0,#0		;store result of pop into R3
	JSR POP
	ADD R4,R0,#0		;store result of pop into R4
	JSR DIV
	JSR PUSH
	LD R7, EVAL_SaveR7
	RET
	
	LD R1,EXP_ASCII
	ADD R1,R0,R1
	BRnp #9			;if R0 is not exponent operand, branch past the exponentiation code
	ST R7, EVAL_SaveR7
	JSR POP
	ADD R3,R0,#0		;store result of pop into R3
	JSR POP
	ADD R4,R0,#0		;store result of pop into R4
	JSR EXP
	JSR PUSH
	LD R7, EVAL_SaveR7
	RET
	
	BRnzp ERROR_MESSAGE	;if we have iterated through every operator and none match, character is invalid

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
PLUS	
	AND R0,R0,#0		;clear R0
	ADD R0,R3,R4		;store R3 + R4 (operands) in R0
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MIN	
	AND R0,R0,#0		;clear R0
	ST R3,OP_SaveR3		;store R3
	NOT R3,R3
	ADD R3,R3,#1		;negate the value in R3
	ADD R0,R3,R4		;add R4 and -R3, store in R0
	LD R3,OP_SaveR3
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MUL	
	AND R0,R0,#0		;clear R0
	ST R3,OP_SaveR3		;store R3
MULT_LOOP
	ADD R0,R0,R4		;add R4 to R0
	ADD R3,R3,xFFFF		;decrement R3
	BRp MULT_LOOP		;if we have not added R4 to R0 R3 times, loop back until we have done so
	LD R3,OP_SaveR3
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
DIV	
	ADD R3,R3,#0		;for branch
	BRz ERROR_MESSAGE	;if division by 0, expression is invalid
	AND R0,R0,#0		;clear R0
	ST R4, OP_SaveR4	;store value of R4 so we don't modify it
	ST R3, OP_SaveR3
	NOT R3,R3
	ADD R3,R3,#1		;negate R3
DIV_LOOP
	ADD R0,R0,#1		;increment R0 for each R4 we subtract from R3
	ADD R4,R3,R4
	BRzp DIV_LOOP		;branch to beginning of loop if R3 is not negative
	ADD R0,R0,xFFFF		;decrement R0, since if R3 is negative we have divided one too many times
	RET
	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
EXP	
	AND R0,R0,#0		;clear R0
	ADD R3,R3,#0		;for branch
	BRz ZERO_EXP		;if exponent is 0, we want result to be 1 so we branch
	ST R7, OP_SaveR7	;store R7, since we will be calling mult
	ST R4, OP_SaveR4	;store R4 since we will be modifying it here
	ST R3, OP_SaveR3	;store R3 since we will be modifying it here
	AND R2,R2,#0		;clear R2 (we'll need it later)
	ADD R1,R3,#0		;store value from R3 into R1 (we will need R3 to be same as R4 when we call mult)
	ADD R1,R1,xFFFF		;decrement R1, since we will want to perform 1 less mult than exponent
	BRnz #1			;if exponent is 1, we want R3 to keep its value (so mult will do R4 * 1 = R4)
	ADD R3,R4,#0		;set R3 to be same as R4 for mult operation to follow
EXP_LOOP
	JSR MUL
	ADD R2,R2,R0		;store value from R0 into R2 so we can call mult again
	ADD R1,R1,xFFFF		;decrement R1 (which records the exponent number, so it's essentially a counter)
	BRp EXP_LOOP

	LD R7, OP_SaveR7
	ADD R0,R2,#0		;store R2 in R0
	LD R4, OP_SaveR4	;load correct value for R4 back in
	LD R3, OP_SaveR3	;load correct value for R3 back in
	BRnzp #1		;branch past ZERO_EXP
ZERO_EXP
	ADD R0,R0,#1		;if exponent is 0, result is always 1

	RET
	
;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACK_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
OP_SaveR7	.BLKW #1	; needed for when we call subroutine inside operand subroutine
EVAL_SaveR7	.BLKW #1	; needed for when we call subroutine from EVALUATE
PRINT_SaveR7	.BLKW #1	; needed for when we call OUT from PRINT_HEX
PRINT_SaveR5	.BLKW #1	; needed since we want R5 to store the result of our operations
OP_SaveR4	.BLKW #1	; needed for when we overwrite R4 in certain operations
OP_SaveR3	.BLKW #1	; needed for when we overwrite R3 in certain operations
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;
ZERO_ASCII	.FILL xFFD0	; negation of '0' ascii value
NINE_ASCII	.FILL xFFC7	; negation of '9' ascii value
MULT_ASCII	.FILL xFFD6	; negation of '*' ascii value
PLUS_ASCII	.FILL xFFD5	; negation of '+' ascii value
MINUS_ASCII	.FILL xFFD3	; negation of '-' ascii value
DIVIDE_ASCII	.FILL xFFD1	; negation of '/' ascii value
EXP_ASCII	.FILL xFFA2	; negation of '^' ascii value
EQUALS_ASCII	.FILL xFFC3	; negation of '=' ascii value
SPACE_ASCII	.FILL xFFE0	; negation of ' ' ascii value
LETTER_OFFSET	.FILL x0037	; offset of hex value from ascii value for letters
NUMBER_OFFSET	.FILL x0030	; offset of hex value from ascii value for numbers
ERROR_STRING	.STRINGZ "Invalid Expression"	;string for our invalid expression


.END
