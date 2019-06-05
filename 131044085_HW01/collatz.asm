        ; 8080 assembler code
        .hexfile Collatz.hex
        .binfile Collatz.com
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
LOAD_EXEC	equ 5
PROCESS_EXIT    equ 9
SET_QUANTUM     equ 6

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
	mvi d, 0        	; 0 karsilastirma icin
	mvi h, 1
	mvi c, 0
	MVI A, READ_B		; store the OS call code to A
	call GTU_OS			; call the OS	
	
start:	
	MOV A, B
loop:
	INR C
	DCR A
	DCR A
	CMP H
	JZ odd     	; C==1 go to first loop
	JMP even	 	; <
	JNC loop
	
odd:
	MVI A, PRINT_B		; store the OS call code to A
	call GTU_OS			; call the OS	
	MOV A,B
	ADD A
	ADD A
	INR A
	MOV B, A
	MVI C,0
	MVI A,1
	CMP B
	JZ stop
	JMP start

even:
	MVI A, PRINT_B		; store the OS call code to A
	call GTU_OS			; call the OS	
	MOV A,B
	SUB C
	MVI C, 0
	MOV A,B
	MVI A,1
	CMP B
	JZ stop
	JMP start


stop:
	hlt		; end program