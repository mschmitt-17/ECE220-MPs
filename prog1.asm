; partners: asr9, jrvasko2
;
; The code given to you here implements the histogram calculation that 
; we developed in class.  In programming lab, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of 
; code to print the histogram to the monitor.
;
; If you finish your program, 
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string 
; terminated by a NUL character.  Lower case and upper case should 
; be counted together, and a count also kept of all non-alphabetic 
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance) 
; should be stored starting at x3F00, with the non-alphabetic count 
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram
	
	; fill the histogram with zeroes 
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop

; you will need to insert your code to print the histogram here

PRINT_HIST

; For each bin in the histogram, using predetermined offsets, first store the ascii value of the character
; in R0 and print, store the ascii value for space in R0 and print. After this, loop through 4 sets of 4 bits
; corresponding to the histogram value for each bin, printing the ascii value corresponding to the hex value
; for each set of 4 bits (again using predetermined offsets to get the correct ascii value for letters and
; numbers). After this, print a newline. Again, we will repeat this process for all 27 bins.

;Table of registers
;	R0 is temporary register for printing characters to the screen
;	R1 is the counter for the current histogram bucket
;	R2 is the histogram starting address
;	R3 contains the histogram data for each bin
;	R4 is the counter for each set of 4 bits in a hex value
;	R5 is the counter for each individual bit in a set of 4 bits
;	R6 is a temporary register
;	R7 is unused

	LD R2,HIST_ADDR		; load ascii code for histogram bin label into register
	AND R1,R1,#0		; clear R1
PRINT_LOOP
	LD R6,ASCII_OFFSET	; load R6 with our ascii offset value
	ADD R0,R1,R6		; store R6(ascii offset) + R1(bin number) to get ascii code of character
	OUT			; print character corresponding to ascii code to screen
	LD R0,ASCII_SPACE	; load ascii code for space character into register
	OUT			; print space character to screen
; code hex value in register to number print here
	AND R4,R4,#0		; clear R4
	ADD R4,R4,#4		; set R4 to 4, the number of sets of 4 bits we need to analyze
	ADD R6,R2,R1		; load address of current bucket in R6
	LDR R3,R6,#0		; load data in address of current bucket into R3
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

	LD R0, NEWLINE		; load newline character 
	OUT			; print newline character
	ADD R1,R1,#1		; increment counter
	LD R6,NUM_BINS		; load R6 with number of bins
	NOT R6,R6		; negate R6
	ADD R6,R6,#1		; add 1 to R6 (so R6 is now negative number of bins)
	ADD R6,R1,R6		; load bin counter + negative number of bins into R6
	BRn PRINT_LOOP		; if R6 is less than or equal to number of bins, we need to keep looping

DONE	HALT			; done


; the data needed by the program
NUM_BINS	.FILL #27	; 27 loop iterations
NEG_AT		.FILL xFFC0	; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6	; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0	; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00     ; histogram starting address
ASCII_OFFSET	.FILL x0040	; R1 + ASCII_OFFSET = ASCII code of character we wish to print
ASCII_SPACE	.FILL x0020	; ASCII code for space character (we will need to print this often)
LETTER_OFFSET	.FILL x0037	; offset of hex value from ascii value for letters
NUMBER_OFFSET	.FILL x0030	; offset of hex value from ascii value for numbers
NEWLINE		.FILL x000A	; ASCII code for newline (which we will also be printing often)
STR_START	.FILL x4000	; string starting address

; for testing, you can use the lines below to include the string in this
; program...
; STR_START	.FILL STRING	; string starting address
; STRING		.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

	.END
