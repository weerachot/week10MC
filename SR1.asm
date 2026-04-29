	ORG     0000H
    	JMP     0100H

    	ORG     0100H
    	MOV     SP,#3FH
    	CLR     EA
    	SETB    P1.0
    	JB      P1.0,$
    	CALL    Delay500ms

    	MOV     TMOD,#00100001B     ; T1M2, T0M1
    	MOV     SCON,#01010000B     ; Serial Mode1
    	MOV     TH1,#0FDH           ; 9600 bps @ 18.432MHz
    	SETB    TR1

    	MOV     R7,#10

Main_Loop:
    	MOV     DPTR,#NAME_ID
    	CALL    SendString
    	MOV     A,#0DH
    	MOV     SBUF,A
    	JNB     TI,$
    	CLR     TI
    	
    	MOV     A,#0AH
    	MOV     SBUF,A
    	JNB     TI,$
    	CLR     TI

    	CALL    Delay500ms
    	DJNZ    R7,Main_Loop

    	MOV     DPTR,#IWA
    	CALL    SendString

    	MOV     A,#0DH
    	MOV     SBUF,A
    	JNB     TI,$
    	CLR     TI
    	MOV     A,#0AH
    	MOV     SBUF,A
    	JNB     TI,$
    	CLR     TI

Stop:
    	JMP     Stop

SendString:
    	CLR     A
    	MOVC    A,@A+DPTR
    	JZ      SendDone
    	MOV     SBUF,A
    	JNB     TI,$
    	CLR     TI
    	INC     DPTR
    	JMP     SendString
SendDone:
    	RET

; --- Delay 500ms @ 18.432MHz ---
; 1 cycle = 0.5425us ? 500ms ˜ 921,600 cycles
; ??? 3 ????: R4 x R5 x R6 = 9 x 100 x 100 = 90,000 loops x ~10 cycle
Delay500ms:
    	MOV     R4,#9
D1: 	MOV     R5,#100
D2: 	MOV     R6,#100
D3: 	DJNZ    R6,D3
    	DJNZ    R5,D2
    	DJNZ    R4,D1
    	RET

NAME_ID:    DB  'B6732493-Mr.Weerachot Hongkongka',00H
IWA:        DB  'I Want A',00H

    	END