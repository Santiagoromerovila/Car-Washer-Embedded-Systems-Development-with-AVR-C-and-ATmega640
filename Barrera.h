#ifndef BARRERA_H_
#define BARRERA_H_
// === Pines y definiciones ===

#define MOTOR_BARRERA_PORT PORTD
#define MOTOR_BARRERA_DDR  DDRD
#define MOTOR_BARRERA_PIN  PD4

#define SENSOR_SO1_DDR DDRD
#define SENSOR_SO1_PIN PD0

#define SENSOR_SO2_DDR DDRD
#define SENSOR_SO2_PIN PD1

#define SENSOR_SW1_DDR DDRD
#define SENSOR_SW1_PIN PD2

#define MOTOR_RODILLO_PORT PORTD
#define MOTOR_RODILLO_DDR  DDRD
#define MOTOR_RODILLO_PIN  PD5

#define SENSOR_SO6_DDR DDRD
#define SENSOR_SO6_PIN PD6

#define LED_DDR  DDRL
#define LED_PORT PORTL
#define LED_PIN  PL7

#define TIEMPO_ANTIREBOTE_SW1 50

extern uint32_t tiempo_actual			;
extern uint8_t homing_completado		;

extern uint8_t flanco_sw1_detectado	;
extern uint32_t tiempo_flanco_sw1		;

extern uint8_t contador_pulsos_subida	;
extern uint8_t contador_pulsos_bajada	;

extern uint8_t activar_rodillo		;
extern uint32_t tiempo_rodillo_inicia	;

// === LED ===
extern uint8_t modo_led				;
extern uint8_t modo_led_anterior		;
extern uint8_t led_activo				;
extern uint16_t contador_led_ms		;
extern uint16_t contador_led_parpadeo	;

// === Estados ===

enum EstadoBarrera { B_PARADA, B_SUBIENDO, B_BAJANDO, B_INICIO };

enum EstadoRodilloEntrada { R_PARADO, R_ENCENDIDO };

void mover_barrera();

void parar_barrera();


#endif