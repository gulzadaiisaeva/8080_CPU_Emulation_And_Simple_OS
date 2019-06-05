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

	;This program prints a null terminated string to the screen
polindrome:   db 'Palindrome',00AH,00H       ; null terminated string 
nopolindrome: db 'Not Palindrome',00AH,00H ; null terminated string 

begin:
	LXI SP,stack 	; always initialize the stack pointer
	mvi d, 00H 	; null character
	mvi a, 50
	mvi e, 0    ; for count
	mov b, a
	mov c, a 

	JMP getinput	; get input

start:
	mov h, b
	MOV l, b
	
findsize:
	LDAX B 			; A = input
	CMP D 			; A == null character
	JZ divide	    
	INX B 			; BC + 1
	INR E 			; count + 1
	JNC findsize 	

divide:
	MOV A, E
	DCX B 			
	MOV D, H 		; 0 index 
	MOV E, L 		; last index
	RAR 			; middle
	INR A 			
	MOV H, A	

process: 
	DCR H 			; from middle until 0
	MVI A, 0		; 
	CMP H 			; A?H
	JZ ispol        ; 
	JMP findindex
compare:	
	CMP L  			; 
	JMP incrindex
check:	
	JZ process 		; compare first index with last index
	JMP isnopol  	

findindex:
	LDAX B 			; get ++ first index
	MOV L, A 		
	LDAX D 			; get -- last index
	JMP compare

getinput:
	MVI A, READ_STR    ; BC = input
	CALL GTU_OS
	JMP	start

isnopol:
	LXI B, nopolindrome	; put the address of string in registers B and C
	MVI A, PRINT_STR	; store the OS call code to A
	call GTU_OS	; call the OS
	JMP stop

incrindex:
	INX D 			; prep last index
	DCX B 			; next first index
	JMP check

ispol:
	LXI B, polindrome	; put the address of string in registers B and C
	MVI A, PRINT_STR	; store the OS call code to A
	call GTU_OS	; call the OS
	JMP stop


stop:
	hlt