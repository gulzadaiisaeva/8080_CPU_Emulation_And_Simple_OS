        ; 8080 assembler code
        .hexfile ShowPrimes.hex
        .binfile ShowPrimes.com
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


stringprime:	dw 'prime',00AH,00H

begin:
	
	mvi d, 0          ; 0 karsilastirma icin
	mvi h, 1
	mvi l, 0

	MVI A, READ_B
	CALL GTU_OS
	MOV B,E

	MOV A, E          ; input == 0
	CMP D
	JZ stop

	MOV A, E          ; input == 1
	CMP H
	JZ stop

	MVI A,2           ; input == 2
	CMP E
	JZ istwo

floop:                ; loop 1 dis loop
	MVI B, 2     
	MOV A,E
	CMP B
	JZ istwo
	JMP sloop

fcontinue:
	MOV A, L            ; asal ise bolen 0
	CMP D
	JZ print
continue:
	MVI L, 0	
	MOV A, E

	MOV B, E            ; sayilari ekrana basmak  
	MVI A, PRINT_B	    ; store the OS call code to A
	call GTU_OS	        ; call the OS

	DCR E
	DCR A  		        ; dis loop eksiltme --A
	CMP H  		        ; A==1 A<1 A>1
	JZ stop		        ; 1 ise bitir
	JMP floop			; degilse dis loop devam et

sloop: 					; loop 2 ic loop
	MOV A,E  			
	MOV C,E  			
	DCR C 	 			; C = (disloop-1)
prime:
	MOV A,E  			; 
	JMP isprime			; asal midir
scont:
	MOV A, C     		; A = C
	DCR C
	DCR A               ; dis loop --C
	CMP H        		; C==1 C<1 C>1
	JZ fcontinue     	; C==1 go to first loop
	JNC prime	 		; >
	JMP fcontinue	 	; <


isprime:
	SUB C  	    		; A-C=A  3-2 =1
	CMP C  		 		; cikan cikarilandan buyuk mu
	JZ esit      		; 
	JNC isprime	 		; tekrar cikar A>C
	JMP kucuk 	 		; kucuktur A<C
	

esit:                   ; esit ise
	INR L               ; ++L flag
	JMP fcontinue    	; == 0 ise asal degildir
kucuk:
	JMP scont	

print:                  ; prime diye yazdirma
	MOV L, B
	LXI B, stringprime	; 
	MVI A, PRINT_STR	; store the OS call code to A
	call GTU_OS			; call the OS
	MOV B,L
	JMP continue	

istwo:
	LXI B, stringprime	; 
	MVI A, PRINT_STR	; store the OS call code to A
	call GTU_OS			; call the OS
	MVI B, 2
	MVI A, PRINT_B		; store the OS call code to A
	call GTU_OS			; call the OS	
	JMP stop

stop:
	MVI A, PROCESS_EXIT    ; BC = input
    CALL GTU_OS
