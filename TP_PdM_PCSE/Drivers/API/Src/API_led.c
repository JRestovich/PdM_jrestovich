/*
 * API_led.c
 *
 *  Created on: Apr 10, 2026
 *      Author: joaquin
 */

#include "API_led.h"

#define FREQ_1Hz 1000

void _initPortClock(void);

void API_LED_Init(led_t* led, GPIO_TypeDef *port, uint16_t pin) {
	if (led == NULL) {
		return;
	}
	_initPortClock();
    led->pin = pin;
    led->port = port;

	GPIO_InitTypeDef GPIO_InitStruct;

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : pin */
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(port, &GPIO_InitStruct);

	led->mode = FIX;
	delayInit(&led->delay, FREQ_1Hz);
}

void API_LED_On(led_t* led) {
	if (led == NULL) {
		return;
	}
	HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}

void API_LED_Off(led_t* led) {
	if (led == NULL) {
		return;
	}
	HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}

void API_LED_Toggle(led_t* led) {
	if (led == NULL) {
		return;
	}
	HAL_GPIO_TogglePin(led->port, led->pin);
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

void _initPortClock(void) {
	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOH_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

}
