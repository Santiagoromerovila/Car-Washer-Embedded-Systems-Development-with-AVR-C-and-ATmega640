

#ifndef INIT_H_
#define INIT_H_

// Pines sensores rodillo (puerto K)

#define SO3_PIN  0
#define SO4_PIN  1
#define SO5_PIN  2

// Pines motores rodillo (puerto L)

#define M3_DIR_PIN 0
#define M3_EN_PIN  2
#define M4_DIR_PIN 4
#define M4_EN_PIN  6

// Pines sensores secador (puerto K)

#define SO7_PIN  3
#define SO8_PIN  4
#define SO9_PIN  5

// Pines motor secador

#define M5_DIR_PIN 1 // puerto L
#define M5_EN_DDR  DDRB

// Pines sensores FC (puerto K)

#define SW2_PIN  6
#define SW3_PIN  7


#define M5_EN_PIN  4 // OC2A (PB4)

void setup_pwm_secador();

void setupMlimpieza();

void interrupcion_init();

void correa_pwm_initc();

void semaforo_init();



void config_led();

void config_barrera();

void config_rodillo();

void config_sensores();



#endif