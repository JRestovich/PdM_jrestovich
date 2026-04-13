/**
 * @file API_led.c
 * @brief Implementacion de la logica de control de LEDs.
 *
 * Este archivo implementa la capa de alto nivel del modulo de LEDs. Aqui se
 * resuelve la logica de operacion, el modo de funcionamiento y el temporizado
 * del parpadeo, mientras que el acceso al hardware GPIO se abstrae a traves de
 * `API_led_port`.
 */

/********************************************************/
/* Includes                                             */
/********************************************************/

#include "API_led.h"

/********************************************************/
/* Defines                                              */
/********************************************************/

#define FREQ_1Hz 1

/********************************************************/
/* Implementacion de Funciones Publicas                 */
/********************************************************/

void API_LED_Init(led_t* led, GPIO_TypeDef *port, uint16_t pin) {
	if (led == NULL) {
		return;
	}
	API_LED_port_Init(&led->hwLed, port, pin);

	led->mode = FIX;
	API_LED_SetBlinkFreq(led, FREQ_1Hz);
}

void API_LED_On(led_t* led) {
	if (led == NULL) {
		return;
	}
	API_LED_port_On(&led->hwLed);
}

void API_LED_Off(led_t* led) {
	if (led == NULL) {
		return;
	}
	API_LED_port_Off(&led->hwLed);
}

void API_LED_Toggle(led_t* led) {
	if (led == NULL) {
		return;
	}
	API_LED_port_Toggle(&led->hwLed);
}

void API_LED_Engine(led_t* led) {
	if (led == NULL) {
		return;
	}
	if (led->mode != BLINK) {
		return;
	}

	if (delayRead(&led->delay)) {
		API_LED_Toggle(led);
	}
}

void API_LED_SetMode(led_t* led, led_mode_e mode) {
	if (led == NULL) {
		return;
	}
	if (led->mode == mode) {
		return;
	}

	led->mode = mode;
	API_LED_Off(led);
}

void API_LED_SetBlinkFreq(led_t* led, uint32_t freq) {
	uint32_t halfPeriodMs;

	if (led == NULL) {
		return;
	}
	if (freq == 0U) {
		return;
	}

	halfPeriodMs = 1000U / (2U * freq);
	if (halfPeriodMs == 0U) {
		halfPeriodMs = 1U;
	}

	delayWrite(&led->delay, halfPeriodMs);
}
