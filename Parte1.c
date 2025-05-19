#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "Parte1.h"
#include "init.h"



// --- Funciones de control del rodillo ---



// --- Funciones de control del secador ---

void secador_subir() {
	PORTL |= (1 << M5_DIR_PIN);
	OCR2A = 80;
}

void secador_bajar() {
	PORTL &= ~(1 << M5_DIR_PIN);
	OCR2A = 80;
}

void secador_detener() {
	OCR2A = 0;
}


// --- Actualizacion de estados con antirrebotes ---

void actualizar_estado_rodillo(uint8_t estado_pines) {
	if (!(estado_pines & (1 << SO3_PIN)) || !(estado_pines & (1 << SO5_PIN))) {
		estado = SUBIENDO;
		contador_inactividad = 0;
	} else if (!(estado_pines & (1 << SO4_PIN)) &&
	(estado_pines & (1 << SO3_PIN)) &&
	(estado_pines & (1 << SO5_PIN))) {
		estado = QUIETO;
		contador_inactividad = 0;
		
	}
	else if (!(estado_pines & (1 << SW2_PIN))) {
		estado = STOP;
		contador_inactividad = 0;
	}

	else {
		estado = BAJANDO;
		contador_inactividad = 0;
	}
	
}

void actualizar_estado_secador(uint8_t estado_pines) {
	if (!(estado_pines & (1 << SO7_PIN)) || !(estado_pines & (1 << SO9_PIN))) {
		estado_S = S_SUBIENDO;
	} else if (!(estado_pines & (1 << SO8_PIN)) &&
	(estado_pines & (1 << SO7_PIN)) &&
	(estado_pines & (1 << SO9_PIN))) {
		estado_S = S_QUIETO;
	}
	else if (!(estado_pines & (1 << SW3_PIN))) {
		estado_S = S_STOP;
	}
	else {
		estado_S = S_BAJANDO;
	}
	
}



