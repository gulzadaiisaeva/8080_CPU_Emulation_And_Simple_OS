        ; 8080 assembler code
        .hexfile Factorize.hex
        .binfile Factorize.com
        ; try "hex" for downloading in hex format
        .download bin  
        .objcopy gobjcopy
        .postbuild echo "OK!"
        ;.nodump

	; OS call list
PRINT_B		equ 4
PRINT_MEM	equ 3
READ_B		equ 7
READ_MEM	equ 2
PRINT_STR	equ 1
READ_STR	equ 8

	; Position for stack pointer
stack   equ 0F000h

	org 000H
	jmp begin

	; Start of our Operating System
GTU_OS:	PUSH D
	push D
	push H
	push psw
	nop	; This is where we run our OS in C++, see the CPU8080::isSystemCall()
		; function for the detail.
	pop psw
	pop h
	pop d
	pop D
	ret
	; ---------------------------------------------------------------
	; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE        

	;This program adds numbers from 0 to 10. The result is stored at variable
	; sum. The results is also printed on the screen.

sum	ds 2 ; will keep the sum

begin:
	LXI SP,stack 		; always initialize the stack pointer
	mvi d, 0        	; 0 karsilastirma icin

	MVI A, READ_B		; store the OS call code to A
	call GTU_OS			; call the OS	
	MOV E, B

	MOV B, E
	MVI A, PRINT_B		; store the OS call code to A
	call GTU_OS			; call the OS	

	MOV C,E  			; C dongu 
sloop: 
	DCR C    			; --C
	MOV A, C
	CMP D               ; if C==0 go to stop
	JZ stop             

	MOV A, E
iszero:                 ; bolunur mu bolunmez mi
	SUB C  	    		; A - C = A
	CMP C  		 		; A ? C
	JZ esit     		; A==C
	JNC iszero	 		; tekrar cikar A>C
	JMP kucuk 	 		; kucuktur A<C

esit:                  
	MOV B,C
	MVI A, PRINT_B		; store the OS call code to A
	call GTU_OS			; call the OS	
	JMP sloop    		; 
kucuk:
	JMP sloop	


stop:
	hlt		; end program