ORG 00H
CLR P2.3
MOV DPTR,#CODE_LED
MOV R7,#0
MAIN:	
	MOV A,R7 
	MOVC A,@A+DPTR
	MOV P1,A
	JNB P3.5, INC_CHECK
	JNB P3.3, DEC_CHECK
	JNB P3.2, START_TIMER
	JNB P3.4, RESET
	SJMP MAIN

INC_CHECK:
	JNB P3.5,$
	CJNE R7,#9,INC_TIMER
	SJMP MAIN

INC_TIMER:
INC R7
SJMP MAIN

RESET: 
	JNB P3.4,$
	MOV R7,#0
	SJMP MAIN
	
DEC_CHECK:
	JNB P3.3,$
	CJNE R7,#0,DEC_TIMER
	SJMP MAIN

DEC_TIMER:
	DEC R7
	SJMP MAIN

START_TIMER:
	JNB P3.2,$
	CJNE R7,#0,DEC_LOOP

DEC_LOOP:
	MOV R6,#100D
	MOV A,R7 
	MOVC A,@A+DPTR
	MOV P1,A
	CALL DELAY_SECOND
	DJNZ R7,DEC_LOOP
	CALL BUZZER
	SJMP MAIN

CODE_LED:
	DB 0XC0,0XF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0X90

DELAY_SECOND: 
    CALL DELAY
    DJNZ R6, DELAY_SECOND
    RET

BUZZER:
	MOV A,R7 
	MOVC A,@A+DPTR
	MOV P1,A
	CLR P3.6
LOOP:
	JB P3.4, LOOP
	SETB P3.6
	SJMP MAIN
	
DELAY: 
    MOV TMOD,#00000001B 
    MOV TH0,#0D8H
    MOV TL0,#0F0H
    SETB TR0 
HERE: 
    JNB TF0,HERE 
    CLR TR0 
    CLR TF0 
    RET
	
END