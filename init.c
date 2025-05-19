#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "init.h"
#include "Barrera.h"


void setup_pwm_secador() {
	
	M5_EN_DDR |= (1 << M5_EN_PIN);
	TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B = (1 << CS22);  // prescaler 64
	OCR2A = 80;
	sei();
}

//Funciones inicialización cinta

// Inicializa el Timer1 para Fast PWM en OC1A 
void correa_pwm_initc() {
	
	// Configuracion OC1A como salida
	DDRB |= (1 << PB5);
	DDRB |= (1 << PB7);
	
	// Configuracion Timer1 
	TCCR1A = (1 << COM1A1) | (1 << WGM11); // Fast PWM, TOP=ICR1
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8
	ICR1 = 9999; // TOP → frecuencia PWM
	

	OCR1A = 0;  
	
	PORTB |= (1 << PB7); 
	
	//DDRL |= (1 << PL3);
	sei();
}

// Timer0 para ANTIRREBOTES y INACTIVIDAD (10ms)

void setup_timer0() {
	
	TCCR0A |=(1 << WGM01);  
	OCR0A =156;         
	TIMSK0 |= (1 << OCIE0A);
	TCCR0B |= ((1 << CS02) | (1 << CS00));
	sei();
}

// Configuración inicial Motores Limpieza

void setupMlimpieza() {
	
	// Sensores
	DDRK &= ~((1 << SO3_PIN) | (1 << SO4_PIN) | (1 << SO5_PIN) | (1 << SO7_PIN) | (1 << SO8_PIN) | (1 << SO9_PIN)| (1 << SW2_PIN) | (1 << SW3_PIN));

	// Motores
	DDRL |= (1 << M3_DIR_PIN) | (1 << M3_EN_PIN)| (1 << M4_EN_PIN) | (1 << M5_DIR_PIN);
	
	DDRL &= ~(1 << M4_DIR_PIN);
	
	PORTK |= (1 << SO3_PIN) | (1 << SO4_PIN) | (1 << SO5_PIN) |
	(1 << SO7_PIN) | (1 << SO8_PIN) | (1 << SO9_PIN) |
	(1 << SW2_PIN) | (1 << SW3_PIN);


	PORTL &= ~((1 << M3_EN_PIN) | (1 << M4_EN_PIN));

	setup_pwm_secador();
	setup_timer0();

	 // Habilitar interrupciones globales
}



//Funciones de inicializacion semaforo
void semaforo_init() {
	cli();
	DDRL |= (1 << PL3) | (1 << PL5);  // Configurar PK1 y PK2 como salida
	sei();
}

void interrupcion_init(){
	cli();
	DDRB &= ~(1 << PB0);
	DDRB &= ~(1 << PB1);
	DDRB &= ~(1 << PB2);
	
	//PORTK |= (1 << PK3) | (1 << PK4);
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1<< PCINT0) | (1<< PCINT1) | (1<< PCINT2);
	
	sei();
	
}



void config_led() {
	LED_DDR |= (1 << LED_PIN);
	LED_PORT &= ~(1 << LED_PIN);
}

void config_barrera() {
	MOTOR_BARRERA_DDR |= (1 << MOTOR_BARRERA_PIN);
	MOTOR_BARRERA_PORT &= ~(1 << MOTOR_BARRERA_PIN);
}

void config_rodillo() {
	MOTOR_RODILLO_DDR |= (1 << MOTOR_RODILLO_PIN);
	MOTOR_RODILLO_PORT &= ~(1 << MOTOR_RODILLO_PIN);
	SENSOR_SO6_DDR &= ~(1 << SENSOR_SO6_PIN);
}

void config_sensores() {
	SENSOR_SO1_DDR &= ~(1 << SENSOR_SO1_PIN);
	SENSOR_SO2_DDR &= ~(1 << SENSOR_SO2_PIN);
	SENSOR_SW1_DDR &= ~(1 << SENSOR_SW1_PIN);

	// INT0: SO1 - flanco cualquiera
	EICRA |= (1 << ISC00);
	EICRA &= ~(1 << ISC01);

	// INT1: SO2 - flanco de subida
	EICRA |= (1 << ISC10) | (1 << ISC11);

	// INT2: SW1 - flanco cualquiera
	EICRA |= (1 << ISC20);
	EICRA &= ~(1 << ISC21);

	EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT2);
}