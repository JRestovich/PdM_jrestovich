/**
 * @file API_led_port.c
 * @brief Implementacion de la capa de acceso a hardware para LEDs.
 *
 * Este archivo implementa la capa de bajo nivel del modulo de LEDs. Aqui se
 * concentra exclusivamente la configuracion y el manejo del hardware GPIO
 * asociado a cada LED, sin incorporar logica de temporizacion ni de modos de
 * funcionamiento.
 */

/********************************************************/
/* Includes                                             */
/********************************************************/

#include "API_led_port.h"

/********************************************************/
/* Declaracion de Funciones Privadas                    */
/********************************************************/

/**
 * @brief Habilita el clock del puerto GPIO utilizado por el LED.
 *
 * Activa unicamente el reloj del puerto GPIO asociado al descriptor de
 * hardware recibido para poder configurar y operar fisicamente ese LED.
 */
static void _initPortClock(led_port_t* led);

/********************************************************/
/* Implementacion de Funciones Publicas                 */
/********************************************************/

void API_LED_port_Init(led_port_t* led, GPIO_TypeDef *port, uint16_t pin) {
	if (led == NULL) {
		return;
	}

    led->pin = pin;
    led->port = port;
    _initPortClock(led);

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

bool_t API_LED_port_GetValue(led_port_t* led) {
	if (led == NULL || led->port == NULL) {
		return false;
	}

	return HAL_GPIO_ReadPin(led->port, led->pin) == GPIO_PIN_SET;
}

/********************************************************/
/* Implementacion de Funciones Privadas                 */
/********************************************************/

static void _initPortClock(led_port_t* led) {
	if (led == NULL || led->port == NULL) {
		return;
	}

	if (led->port == GPIOA) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
	} else if (led->port == GPIOB) {
		__HAL_RCC_GPIOB_CLK_ENABLE();
	} else if (led->port == GPIOC) {
		__HAL_RCC_GPIOC_CLK_ENABLE();
	} else if (led->port == GPIOD) {
		__HAL_RCC_GPIOD_CLK_ENABLE();
	} else if (led->port == GPIOE) {
		__HAL_RCC_GPIOE_CLK_ENABLE();
	} else if (led->port == GPIOF) {
		__HAL_RCC_GPIOF_CLK_ENABLE();
	} else if (led->port == GPIOG) {
		__HAL_RCC_GPIOG_CLK_ENABLE();
	} else if (led->port == GPIOH) {
		__HAL_RCC_GPIOH_CLK_ENABLE();
	}
}
