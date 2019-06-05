        ; 8080 assembler code
        .hexfile sender.hex
        .binfile sender.com
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
MUTEX_ADDRESS equ 09C40h	;Store mutex  0
FULL_ADDRESS equ 09C41h		;Store full semaphore 1 
EMPTY_ADDRESS equ 09C42h	;Store empty semaphore 2

begin:
	MVI L, 0
	MVI E,200d

firstloop:
	MVI A, RAND_INT				; store the OS call code to A
	call GTU_OS					; call the OS

waitEmpty:
	MVI C, 2					; Loaad empty
	MVI A, WAIT
	call GTU_OS

waitMutex:
	MVI C, 0                    ; C = id of mutex
	MVI A, WAIT
	call GTU_OS

produce:
	
	MVI A, SEND
	call GTU_OS

signalMutex:
	MVI C, 0                    ; C = id of mutex
	MVI A, SIGNAL               ; mutex=1;
	call GTU_OS

signalFull:
	MVI C, 1                    ; C = id of full
	MVI A, SIGNAL               ; full++
	call GTU_OS

	DCR E						; --C
	JNZ firstloop					; goto loop if C!=0
	
	MVI E,200d

secondloop:
	MVI A, RAND_INT				; store the OS call code to A
	call GTU_OS					; call the OS

waitEmpty2:
	MVI C, 2					; Loaad empty
	MVI A, WAIT
	call GTU_OS

waitMutex2:
	MVI C, 0                    ; C = id of mutex
	MVI A, WAIT
	call GTU_OS

produce2:
	MVI A, SEND
	call GTU_OS

signalMutex2:
	MVI C, 0                    ; C = id of mutex
	MVI A, SIGNAL               ; mutex=1;
	call GTU_OS

signalFull2:
	MVI C, 1                    ; C = id of full
	MVI A, SIGNAL               ; full++
	call GTU_OS

	DCR E						; --C
	JNZ secondloop					; goto loop if C!=0

	MVI A,PROCESS_EXIT
	call GTU_OS