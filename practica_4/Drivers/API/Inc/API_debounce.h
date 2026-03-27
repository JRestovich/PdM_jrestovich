/*
 * API_debounce.h
 *
 *  Created on: Mar 26, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

void debounceFSM_init();	// debe cargar el estado inicial

void debounceFSM_update();	// debe leer las entradas, resolver la lógica de
							// transición de estados y actualizar las salidas

debounceState_t debounceFSM_getState();

#endif /* API_INC_API_DEBOUNCE_H_ */
