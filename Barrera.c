#include <avr/io.h>
#include <avr/interrupt.h>
#include "init.h"
#include "Barrera.h"
// === Configuración hardware ===



void mover_barrera() {
	MOTOR_BARRERA_PORT |= (1 << MOTOR_BARRERA_PIN);
}

void parar_barrera() {
	MOTOR_BARRERA_PORT &= ~(1 << MOTOR_BARRERA_PIN);
}

