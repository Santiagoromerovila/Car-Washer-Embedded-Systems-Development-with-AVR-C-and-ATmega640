


.global correa_pwm_init
; Definiciones de registros y bits para ATmega640
.equ DDRB    , 0x04
.equ PORTB   , 0x05
.equ TCCR1A  , 0x80
.equ TCCR1B  , 0x81
.equ ICR1L   , 0x86
.equ ICR1H   , 0x87
.equ OCR1AL  , 0x88
.equ OCR1AH  , 0x89

correa_pwm_init:
    ; Configurar PB5 y PB7 como salidas
    sbi DDRB, 5
    sbi DDRB, 7 

    ; Configurar Timer1 en modo Fast PWM con TOP en ICR1
	lds r16,TCCR1A
    ori r16, (1 << 7) | (1 << 1)
    sts TCCR1A, r16

	lds r16,TCCR1B
    ori r16, (1 << 4) | (1 << 3) | (1 << 1)
    sts TCCR1B, r16

    ; Establecer TOP en ICR1 = 9999
    ldi r16, 0x0F
    sts ICR1L, r16
    ldi r16, 0x27
    sts ICR1H, r16

    ; Establecer OCR1A = 0
    ldi r16, 0
    sts OCR1AL, r16
    sts OCR1AH, r16

    ; Establecer PB7 en alto
    sbi PORTB, 7

    ; Habilitar interrupciones globales
    sei

    ret