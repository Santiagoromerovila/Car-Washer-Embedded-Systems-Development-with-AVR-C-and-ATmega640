#include <avr/io.h>
	
	.global config_timer
	.global encender_rodillo
	.global apagar_rodillo

config_timer:
    ldi r16, 0
    sts TCCR5A, r16
    sts TCCR5B, r16

    ldi r16, 0xF9
    sts OCR5A, r16
    ldi r16, 0X00
    sts OCR5A+1, r16

    lds r16, TCCR5B
    ori r16, (1 << WGM52)
    sts TCCR5B, r16

    lds r16, TCCR5B
    ori r16, (1 << CS51) | (1 << CS50)
    sts TCCR5B, r16

    lds r16, TIMSK5
    ori r16, (1 << OCIE5A)
    sts TIMSK5, r16

    ret

encender_rodillo:
	in   r24, 0x0B          
	ori  r24, (1 << 5)       
	out  0x0B, r24           
	ret

apagar_rodillo:
	in   r24, 0x0B         
	andi r24, ~(1 << 5)     
	out  0x0B, r24           
	ret
