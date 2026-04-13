/**
 * @file API_LED_port.c
 * @brief Implementacion del modulo de control de LEDs.
 *
 * Este archivo define la logica interna necesaria para operar LEDs mediante
 * GPIOs, incluyendo la inicializacion del hardware y el control temporal
 * del parpadeo.
 */

/********************************************************/
/* Includes                                             */
/********************************************************/

#include "API_led_port.h"

/********************************************************/
/* Declaracion de Funciones Privadas                    */
/********************************************************/

/**
 * @brief Habilita los clocks de los puertos GPIO utilizados por el modulo.
 *
 * Activa los relojes de los puertos GPIO necesarios para poder configurar y
 * operar los LEDs.
 */
static void _initPortClock(void);

/********************************************************/
/* Implementacion de Funciones Publicas                 */
/********************************************************/

void API_LED_port_Init(led_port_t* led, GPIO_TypeDef *port, uint16_t pin) {
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
}

void API_LED_port_On(led_port_t* led) {
	if (led == NULL) {
		return;
	}
	HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}

void API_LED_port_Off(led_port_t* led) {
	if (led == NULL) {
		return;
	}
	HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}

void API_LED_port_Toggle(led_port_t* led) {
	if (led == NULL) {
		return;
	}
	HAL_GPIO_TogglePin(led->port, led->pin);
}

/********************************************************/
/* Implementacion de Funciones Privadas                 */
/********************************************************/

static void _initPortClock(void) {
	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOH_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

}
