        ; 8080 assembler code
        .hexfile receiver.hex
        .binfile receiver.com
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
LOAD_EXEC 	equ 5
PROCESS_EXIT	equ 9
SET_QUANTUM 	equ 6
PRINT_WHOLE	equ 10d	
RAND_INT equ 12d
WAIT equ 13d
SIGNAL equ 14d
SEND equ 15d
READ equ 16d
GETCOND equ 17d

	; Position for stack pointer
stack   equ 0F000h

	org 000H
	jmp begin

	; Start of our Operating System
GTU_OS:
	PUSH D
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

MUTEX_ADDRESS equ 09C40h	;Store mutex
FULL_ADDRESS equ 09C41h		;Store full semaphore
EMPTY_ADDRESS equ 09C42h	;Store empty semaphore

begin:
	MVI L, 0
	MVI E, 200d

firstloop:

consume:
	MVI A, READ
	call GTU_OS

	DCR E						; --E
	JNZ firstloop					; goto loop if E!=0


	MVI E, 200d
secondloop:

consume2:
	MVI A, GETCOND
	call GTU_OS
	MVI A, READ
	call GTU_OS


	DCR E						; --E
	JNZ secondloop					; goto loop if E!=0
	

	MVI A,PROCESS_EXIT
	call GTU_OS