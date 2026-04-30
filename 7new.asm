ORG     0000H
        LJMP    MAIN

        ORG     0003H
        LJMP    INT0_ISR

        ORG     0030H

SENSOR  BIT     P3.2
COUNT_L EQU     30H
COUNT_H EQU     31H
DEBOUNCE EQU    32H             ; Flag กันนับซ้ำ

MAIN:
        MOV     TMOD, #20H
        MOV     TH1, #0FDH
        MOV     TL1, #0FDH
        MOV     SCON, #50H
        SETB    TR1

        MOV     COUNT_L, #00H
        MOV     COUNT_H, #00H
        MOV     DEBOUNCE, #00H  ; เคลียร์ flag

        SETB    IT0
        SETB    EX0
        SETB    EA

LOOP:
        MOV     DPTR, #DMSG
        LCALL   SENDSTR
        MOV     A, COUNT_H
        LCALL   SENDHEX
        MOV     A, COUNT_L
        LCALL   SENDHEX
        MOV     A, #0DH
        LCALL   SENDCHAR
        MOV     A, #0AH
        LCALL   SENDCHAR

        LCALL   DELAY500MS
        LJMP    LOOP

;-------------------------------
; INT0 ISR
;-------------------------------
INT0_ISR:
        ; Save registers ที่จะใช้
        PUSH    ACC
        PUSH    PSW

        ; เช็ค debounce flag ถ้ายังอยู่ใน debounce period ให้ออกไปเลย
        MOV     A, DEBOUNCE
        JNZ     ISR_EXIT

        ; Set flag กันนับซ้ำก่อน
        MOV     DEBOUNCE, #01H

        ; เช็คว่า Sensor ยัง LOW จริงมั้ย
        JB      SENSOR, ISR_EXIT

        ; นับ
        INC     COUNT_L
        MOV     A, COUNT_L
        JNZ     ISR_EXIT
        INC     COUNT_H

ISR_EXIT:
        POP     PSW
        POP     ACC
        RETI

;-------------------------------
; Main Loop เคลียร์ debounce flag
; ทุก 500ms (เพียงพอสำหรับ debounce)
;-------------------------------
; ** เพิ่มใน LOOP ก่อน DELAY500MS **
; MOV DEBOUNCE, #00H   เพิ่มบรรทัดนี้!

;-------------------------------
; Delay 500ms - ใช้ R4,R5,R6,R7
;-------------------------------
DELAY500MS:
        MOV     DEBOUNCE, #00H  ; เคลียร์ debounce flag ทุกรอบ
        MOV     R7, #20
D1:     MOV     R6, #250
D2:     MOV     R5, #100
D3:     DJNZ    R5, D3
        DJNZ    R6, D2
        DJNZ    R7, D1
        RET

;-------------------------------
; ส่ง String จาก DPTR
;-------------------------------
SENDSTR:
        CLR     A
        MOVC    A, @A+DPTR
        JZ      SSTR_END
        LCALL   SENDCHAR
        INC     DPTR
        SJMP    SENDSTR
SSTR_END:
        RET

;-------------------------------
; ส่ง 1 ไบต์ออก Serial
;-------------------------------
SENDCHAR:
        MOV     SBUF, A
WAIT_TX:
        JNB     TI, WAIT_TX
        CLR     TI
        RET

;-------------------------------
; ส่ง A เป็น 2 หลัก HEX
;-------------------------------
SENDHEX:
        MOV     B, A
        SWAP    A
        ANL     A, #0FH
        LCALL   HEXCHAR
        LCALL   SENDCHAR
        MOV     A, B
        ANL     A, #0FH
        LCALL   HEXCHAR
        LCALL   SENDCHAR
        RET

HEXCHAR:
        ADD     A, #30H
        CJNE    A, #3AH, HC1
HC1:    JC      HC_END
        ADD     A, #07H
HC_END: RET

DMSG:   DB      'Data Count = ', 00H

        END