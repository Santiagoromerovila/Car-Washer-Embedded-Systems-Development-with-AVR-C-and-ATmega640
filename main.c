/*
 * CarWashingprojectAVR.c
 *
 * Created: 21/04/2025 17:22:15
 * Author : Adrian
 */ 
//libraries
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "init.h"
#include "general.h"
#include "Parte1.h"
#include "Barrera.h"





 int flag = 0 ;
 int Stopsign =0 ;

 
 // ANTIRREBOTES

 
 Grupoantirrebote grupo_rodillo = {
	 .mask = (1 << SO3_PIN) | (1 << SO4_PIN) | (1 << SO5_PIN) | (1 << SW2_PIN),
	 .ultimo_estado = 0xFF,
	 .estado_estable = 0xFF,
	 .contador = 0,
	 .pin_reg = &PINK
 };

 Grupoantirrebote grupo_secador = {
	 .mask = (1 << SO7_PIN) | (1 << SO8_PIN) | (1 << SO9_PIN)  | (1 << SW3_PIN),
	 .ultimo_estado = 0xFF,
	 .estado_estable = 0xFF,
	 .contador = 0,
	 .pin_reg = &PINK
 };

 enum EstadoRodillo estado = PARADO;
 enum EstadoSecador estado_S = S_PARADO;
 uint32_t contador_inactividad = 0;
 
 // === Variables de control globales ===
 
 enum EstadoBarrera estado_barrera = B_PARADA;
 enum EstadoRodilloEntrada estado_rodilloEntrada = R_PARADO;
 
 uint32_t tiempo_actual = 0;
 uint8_t homing_completado = 0;
 
 uint8_t flanco_sw1_detectado = 0;
 uint32_t tiempo_flanco_sw1 = 0;
 
 uint8_t contador_pulsos_subida = 0;
 uint8_t contador_pulsos_bajada = 0;
 
 uint8_t activar_rodillo = 0;
 uint32_t tiempo_rodillo_inicia = 0;
 
 // === LED globals ===
 
 uint8_t modo_led = 2;
 uint8_t modo_led_anterior = 0; 
 uint16_t contador_led_ms = 0;
 uint16_t contador_led_parpadeo = 0;
 uint8_t led_activo				;
 
 uint8_t current_state;
 uint8_t last_state=0x00;
 //Funciones en assembly
 extern void correa_pwm_init     (void);
 extern void rodillo_subir		 (void);
 extern void rodillo_bajar		 (void);
 extern void rodillo_girar		 (void);
 extern void rodillo_parar_giro	 (void);
 extern void rodillo_detener	 (void);
 extern void encender_rodillo    (void);
 extern void apagar_rodillo      (void);
 extern void config_timer        (void);
 
 


int main(void) {
	//Inicializaciones
	
	correa_pwm_init();
	semaforo_init();
	interrupcion_init();
	setupMlimpieza();
	config_led();
	config_timer();
	config_barrera();
	config_rodillo();
	config_sensores();
	//Vector de coches
	
	//Velocidad cinta inicial y semaforo
	OCR1A = 0;
	semaforo(0);
	//Inicializacion barrera
	estado_barrera = B_INICIO;
	mover_barrera();
	estado = PARADO;
	estado_S = S_PARADO;
	
	while(1){
		if(Stopsign == 0){
				switch (estado_barrera) {
			case B_SUBIENDO:
			case B_BAJANDO:
			break;
			case B_INICIO:
			mover_barrera();
			break;
			case B_PARADA: 
			parar_barrera();
			break;
		}
				// Encender rodillo 2s tras subida
				if (activar_rodillo && (tiempo_actual - tiempo_rodillo_inicia >= 2000)) {
					estado_rodilloEntrada = R_ENCENDIDO;
					activar_rodillo = 0;
				}

				// Control del rodillo
				switch (estado_rodilloEntrada) {
					case R_PARADO:
					apagar_rodillo();
					break;
					case R_ENCENDIDO:
					encender_rodillo();
					if (!(PIND & (1 << SENSOR_SO6_PIN))) {
						estado_rodilloEntrada = R_PARADO;
					}
					break;
				}


			
	
	
		if (contador_inactividad > 200) {
			estado = PARADO;
		}
		
		switch (estado_rodilloEntrada) {
					case R_PARADO:
					apagar_rodillo();
					break;
					case R_ENCENDIDO:
					encender_rodillo();
					if (!(PIND & (1 << SENSOR_SO6_PIN))) {
						estado_rodilloEntrada = R_PARADO;
					}
					break;
				}
		// RODILLO
		switch (estado) {
			case PARADO:
			rodillo_bajar();
			rodillo_parar_giro();
			break;
			case STOP:
			rodillo_detener();
			rodillo_parar_giro();
			break;
			case QUIETO:
			rodillo_detener();
			rodillo_girar();
			break;
			case SUBIENDO:
			rodillo_subir();
			rodillo_girar();
			break;
			case BAJANDO:
			rodillo_bajar();
			rodillo_girar();
			break;
		}

		// SECADOR
		switch (estado_S) {
			case S_PARADO:
			secador_bajar();
			break;
			case S_STOP:
			secador_detener();
			break;
			case S_QUIETO:
			secador_detener();
			break;
			case S_SUBIENDO:
			secador_subir();
			break;
			case S_BAJANDO:
			secador_bajar();
			break;
		}

		
		
	
			if(flag == 0){
				semaforo(0);
			}else {
				semaforo(1);
			}
				
			
			
			

		// Antirrebote SW1
		if (flanco_sw1_detectado && (tiempo_actual - tiempo_flanco_sw1 >= TIEMPO_ANTIREBOTE_SW1)) {
			flanco_sw1_detectado = 0;

			if (PIND & (1 << SENSOR_SW1_PIN)) {
				if (estado_barrera == B_SUBIENDO) {
					contador_pulsos_subida++;
					if (contador_pulsos_subida >= 2) {
						parar_barrera();
						estado_barrera = B_PARADA;
						
					}
					} else if (estado_barrera == B_BAJANDO) {
					contador_pulsos_bajada++;
					if (contador_pulsos_bajada >= 2) {
						parar_barrera();
						estado_barrera = B_PARADA;
						activar_rodillo = 1;
						tiempo_rodillo_inicia = tiempo_actual;
					}
				}
			}
		}
			// LED según actividad
				if ((estado_barrera != B_PARADA) || (estado_rodilloEntrada != R_PARADO) || (estado != PARADO) || (estado_S != S_STOP)) {
					modo_led = 1;//modo 0.5s
					} else {
					modo_led = 2;//modo 10s
				}

				if (modo_led != modo_led_anterior) {
					contador_led_ms = 0;
					contador_led_parpadeo = 0;
					led_activo = 0;
					LED_PORT &= ~(1 << LED_PIN);
					modo_led_anterior = modo_led;
				}

				}
			
		}
	
	
	
}

ISR(PCINT0_vect) {
	current_state = PINB;  // estado actual del puerto B
	
	if(!(last_state & (1 << PB2)) && (current_state & (1 << PB2)))
	{
		EmergencyStop();
	}  
	// Detectar si PB0 cambió de 1 a 0 
	if (!(last_state & (1 << PB0)) && (current_state & (1 << PB0))) {
		flag = 1;  
	}

	// Detectar si PB1 cambió de 1 a 0
	if (!(last_state & (1 << PB1)) && (current_state & (1 << PB1))) {
		flag = 0;  
	}

	last_state = current_state;  // Actualizar estado 
	
}

// ISR Timer0 cada 10ms

ISR(TIMER0_COMPA_vect) {
	contador_inactividad++;

	ACTUALIZAR_ANTIRREBOTES(grupo_rodillo, actualizar_estado_rodillo);
	ACTUALIZAR_ANTIRREBOTES(grupo_secador,actualizar_estado_secador);
}

ISR(TIMER5_COMPA_vect) {
	tiempo_actual++;
	contador_led_ms++;

	if (modo_led == 1) {
		if (contador_led_ms >= 500) {
			LED_PORT ^= (1 << LED_PIN);
			contador_led_ms = 0;
		}
		} else if (modo_led == 2) {
		contador_led_parpadeo++;
		if (!led_activo && contador_led_parpadeo >= 10000) {
			LED_PORT |= (1 << LED_PIN);
			led_activo = 1;
			contador_led_ms = 0;
			} else if (led_activo && contador_led_ms >= 500) {
			LED_PORT &= ~(1 << LED_PIN);
			led_activo = 0;
			contador_led_parpadeo = 0;
			contador_led_ms = 0;
		}
	}
}

ISR(INT0_vect) {
	OCR1A = 3000;// activa cinta
	if (PIND & (1 << SENSOR_SO1_PIN)) {
		
		estado_barrera = B_SUBIENDO;
		contador_pulsos_subida = 0;
		mover_barrera();
		} else {
		estado_barrera = B_BAJANDO;
		contador_pulsos_bajada = 0;
		mover_barrera();
	}
	
}

ISR(INT1_vect) {
	if (estado_barrera == B_INICIO) {
		parar_barrera();
		estado_barrera = B_PARADA;
		homing_completado = 1;
	}
}

ISR(INT2_vect) {
	if (PIND & (1 << SENSOR_SW1_PIN)) {
		flanco_sw1_detectado = 1;
		tiempo_flanco_sw1 = tiempo_actual;
	}
}


