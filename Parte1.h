#ifndef PARTE1_H_
#define PARTE1_H_
//Lo de aprenderme a hacer macros para implementar el antirrebotes me lo tienen que explicar...

enum EstadoRodillo { PARADO, QUIETO, SUBIENDO, BAJANDO, STOP };

	
enum EstadoSecador { S_PARADO, S_QUIETO, S_SUBIENDO, S_BAJANDO, S_STOP };
	
extern enum EstadoRodillo estado;
extern enum EstadoSecador estado_S;
extern uint32_t contador_inactividad;

	
void secador_detener();

void secador_bajar();

void secador_subir();



void actualizar_estado_rodillo(uint8_t estado_pines);

void actualizar_estado_secador(uint8_t estado_pines);

#define ACTUALIZAR_ANTIRREBOTES(grupo, actualizar_estado)               \
do {                                                             \
	uint8_t actual = *(grupo.pin_reg) & grupo.mask;              \
	if (actual != grupo.ultimo_estado) {                            \
		grupo.ultimo_estado = actual;                               \
		grupo.contador = 0;                                       \
		} else if (grupo.contador < ANTIRREBOTES_TICKS) {                 \
		grupo.contador++;                                         \
		if (grupo.contador >= ANTIRREBOTES_TICKS) {                   \
			grupo.estado_estable = actual;                         \
			actualizar_estado(grupo.estado_estable);              \
		}                                                        \
	}                                                             \
} while (0)


#define ANTIRREBOTES_TICKS 5 // 50 ms con tick de 10 ms

typedef struct {
	uint8_t mask;
	uint8_t ultimo_estado;
	uint8_t estado_estable;
	uint8_t contador;
	volatile uint8_t *pin_reg;
} Grupoantirrebote;




#endif