/*
 * API_delay.h
 *
 *  Created on: Mar 19, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

/********************************************************/
/* Includes */
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/********************************************************/
/* Declaracion de Tipos Publicos */
typedef uint32_t tick_t;
typedef bool bool_t;

typedef struct {
  tick_t startTime;
  tick_t duration;
  bool_t running;
} delay_t;

/********************************************************/
/* Declaracion de Funciones Publicas */
/**
 * @brief Inicializa una estructura de retardo no bloqueante.
 * @param delay Puntero a la estructura delay_t que se desea inicializar.
 * @param duration Tiempo de retardo en ticks.
 */
void delayInit(delay_t * delay, tick_t duration);

/**
 * @brief Actualiza el estado del retardo y avisa cuando el tiempo configurado finalizo.
 * @param delay Puntero a la estructura delay_t a evaluar.
 * @retval true El retardo finalizo.
 * @retval false El retardo sigue en curso o acaba de comenzar.
 */
bool_t delayRead(delay_t * delay);

/**
 * @brief Modifica la duracion configurada para un retardo.
 * @param delay Puntero a la estructura delay_t a actualizar.
 * @param duration Nuevo tiempo de retardo en ticks.
 */
void delayWrite(delay_t* delay, tick_t duration);

// todo doc
bool_t delayIsRunning(delay_t * delay);

#endif /* API_INC_API_DELAY_H_ */
