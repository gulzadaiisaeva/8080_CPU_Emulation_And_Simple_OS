        ; 8080 assembler code
        .hexfile MicroKernel2.hex
        .binfile MicroKernel2.com
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
HANDLER		equ 15


	; Position for stack pointer
stack   equ 0F000h

	org 000H
	jmp begin

	; Start of our Operating System
GTU_OS:	
	push D
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

	JMP start
	NOP
	NOP
empty: dw 'This' ,00H
polin: dw 'Palindrome.com' ,00H

int_handler:  ; interrupt handler
	DI
	
	MVI A, HANDLER
	CALL GTU_OS

	LHLD 65504
	SPHL        ; SP

	POP psw
	POP H
	POP D
	POP B

	PUSH D
	PUSH H

	LHLD 65514
	XCHG        ; DE

	LHLD 65524  ; HL
  	
	EI
	PCHL


sum: dw 'sum.com' ,00H
show: dw 'ShowPrimes.com',00H
 
start:
  DI	
  LXI SP,stack   ; always initialize the stack pointer

  LXI B, polin    
  LXI H, 480
  MVI A, LOAD_EXEC
  CALL GTU_OS

  LXI B, polin
  LXI H, 752
  MVI A, LOAD_EXEC
  CALL GTU_OS

  LXI B, polin    
  LXI H, 1360
  MVI A, LOAD_EXEC
  CALL GTU_OS

  LXI B, polin    
  LXI H, 1600
  MVI A, LOAD_EXEC
  CALL GTU_OS

  LXI B, polin
  LXI H, 2016
  MVI A, LOAD_EXEC
  CALL GTU_OS

  LXI B, polin    
  LXI H, 2544
  MVI A, LOAD_EXEC
  CALL GTU_OS

  LXI B, polin    
  LXI H, 2960
  MVI A, LOAD_EXEC
  CALL GTU_OS

  LXI B, polin
  LXI H, 3520
  MVI A, LOAD_EXEC
  CALL GTU_OS

  LXI B, polin    
  LXI H, 4032
  MVI A, LOAD_EXEC
  CALL GTU_OS

  LXI B, polin    
  LXI H, 4480
  MVI A, LOAD_EXEC
  CALL GTU_OS

  EI

infiniteLoop:
  PUSH PSW
  POP PSW
  JMP infiniteLoop 

  MVI A, PROCESS_EXIT    ; 
  CALL GTU_OS
  
