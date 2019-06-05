        ; 8080 assembler code
        .hexfile Palindrome.hex
        .binfile Palindrome.com
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

begin:
	mvi h, 1
	mvi b, 7

	MVI A, READ_B
	CALL GTU_OS

start:
	MOV A, B     ; A = 7
 
loop:
	RAR         ;A*2
    JC even     ;B is even
    JMP odd     ;B is odd
	
odd:   ;cift
	MOV A,B
	RAR
	MOV B, A    ; B =  a/2
	MOV D,A      ; C =  a/2
	MVI A, PRINT_B
	CALL GTU_OS
	MOV A,D
	CMP H
	JZ end     ;A==1
	JNC loop   ;A>1

even:   ;tek
	MOV A,B
	ADD B
	ADD B
	ADD H
	MOV B,A
	MOV D, A
	MVI A, PRINT_B
	CALL GTU_OS
	MOV A,D
	CMP H
	JZ end    ;a==1
	JMP loop  ; A>1

end:

	MVI A, PROCESS_EXIT    ; BC = input
    CALL GTU_OS
