#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "general.h"
#include "init.h"
#include "Parte1.h"
#include "Barrera.h"


int emergencyflag = 0;

enum EstadoBarrera estado_barrera;

enum EstadoRodillo estado;
enum EstadoSecador estado_S;


int Stopsign;



void EmergencyStop(){
	if(emergencyflag == 0){
		Stopsign =1;
	//DDRK |= ((1 << SO3_PIN) | (1 << SO4_PIN) | (1 << SO5_PIN) | (1 << SO7_PIN) | (1 << SO8_PIN) | (1 << SO9_PIN));
	DDRK &= ~(1 << PK0);
	DDRD &= ~((1 << PD4) | (1 << PD5));
	DDRB &= ~(1 << PB5);
	DDRL &= ~((1 << M3_EN_PIN) | (1 << M4_EN_PIN) | (1 << M5_EN_PIN));
	OCR1A = 0;
	OCR2A = 0;
	estado_S = S_SUBIENDO;
	//emergencyflag = 1;
	
	//}else{
	//	reset();
	//	emergencyflag = 0;
		
		}
}




void semaforo(int estado) {
	if (estado == 1) {
		PORTL |= (1 << PL3);   // Poner P3 en 1
		PORTL &= ~(1 << PL5);  // Poner P5 en 0
		//PORTL = 0b00001000;
		
		} else{
		   PORTL &= ~(1 << PL3);  // Poner P3 en 0
		   PORTL |= (1 << PL5);   // Poner P5 en 1
		 // PORTL = 0b00100000;
	}
}

void reset(){
	//Inicializaciones
	//correa_pwm_init();
	semaforo_init();
	interrupcion_init();
	setupMlimpieza();
	config_led();
	config_timer();
	config_barrera();
	config_rodillo();
	config_sensores();
	//Vector de coches
	
	//Velocidad cinta inicial
	OCR1A = 0;
	semaforo(0);
	//Inicializacion barrera
	estado_barrera = B_INICIO;
	mover_barrera();
	estado = PARADO;
	estado_S = S_PARADO;
	Stopsign = 0;
}