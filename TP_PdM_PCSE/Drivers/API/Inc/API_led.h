/*
 * API_led.h
 *
 *  Created on: Apr 10, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_LED_H_
#define API_INC_API_LED_H_

#include "stm32f4xx_hal.h"
#include "API_delay.h"

typedef enum {
	FIX = 0,
	BLINK
} led_mode_e;

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	led_mode_e mode;
	delay_t delay;
} led_t;

void API_LED_Init(led_t* led, GPIO_TypeDef *port, uint16_t pin);
void API_LED_On(led_t* led);
void API_LED_Off(led_t* led);
void API_LED_Toggle(led_t* led);
void API_LED_Engine(led_t* led);
void API_LED_SetMode(led_t* led, led_mode_e mode);
void API_LED_SetBlinkFreq(led_t* led, uint32_t freq);

#endif /* API_INC_API_LED_H_ */
