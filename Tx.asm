; Board_01_Sender.asm
T2CON   EQU 0C8H
RCAP2L  EQU 0CAH
RCAP2H  EQU 0CBH

        ORG 0000H
        JMP 0100H

        ORG 0100H
        MOV SCON, #40H      ; Mode 1, 8-bit UART (เฉพาะส่ง)
        MOV RCAP2H, #0FFH    
        MOV RCAP2L, #0DCH    ; 9600 Baud @ 11.088MHz
        MOV T2CON, #34H     

; --- ส่วนการสแกนปุ่มสำหรับบอร์ด 1 ---
Scan_SW:
    ; ตรวจสอบ SW0 (P3.2) -> ส่ง '0'
    JB P3.2, Skip0        ; ถ้า P3.2 เป็น 1 (ไม่กด) ให้ข้าม
    MOV A, #'0'
    CALL Send_Serial      ; ส่ง ASCII '0'
    JNB P3.2, $           ; Wait (รอจนกว่าจะปล่อยปุ่ม)
Skip0:

    ; ตรวจสอบ SW1 (P3.3) -> ส่ง '1'
    JB P3.3, Skip1
    MOV A, #'1'
    CALL Send_Serial
    JNB P3.3, $
Skip1:

    ; ตรวจสอบ SW2 (P3.4) -> ส่ง '2'
    JB P3.4, Skip2
    MOV A, #'2'
    CALL Send_Serial
    JNB P3.4, $
Skip2:

    ; ตรวจสอบ SW3 (P3.5) -> ส่ง '3'
    JB P3.5, Skip3
    MOV A, #'3'
    CALL Send_Serial
    JNB P3.5, $
Skip3:

    JMP Scan_SW           ; วนลูปสแกนต่อไป

Send_Serial:
        MOV SBUF, A
        JNB TI, $
        CLR TI
        RET
        END