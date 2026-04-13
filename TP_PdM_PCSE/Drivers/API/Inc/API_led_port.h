/**
 * @file API_led_port.h
 * @brief Interfaz publica de acceso a hardware para LEDs.
 *
 * Este archivo declara la capa de port del modulo de LEDs. La responsabilidad
 * de esta interfaz es encapsular el acceso al hardware GPIO utilizado para
 * inicializar, encender, apagar y conmutar LEDs fisicos del microcontrolador.
 */

#ifndef API_INC_API_LED_PORT_H_
#define API_INC_API_LED_PORT_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>


typedef bool bool_t;

/**
 * @brief Descriptor de hardware asociado a un LED.
 *
 * Agrupa la informacion fisica necesaria para identificar el GPIO conectado a
 * un LED y operar sobre ese recurso desde la capa de port.
 */
typedef struct {
	GPIO_TypeDef *port;  ///< Puerto GPIO asociado al LED.
	uint16_t pin;        ///< Pin GPIO asociado al LED.
} led_port_t;

/**
 * @brief Inicializa el hardware asociado a un LED.
 *
 * Configura el pin indicado como salida digital, lo lleva al estado apagado y
 * deja cargado el descriptor de hardware correspondiente.
 *
 * @param led Puntero a la estructura del LED a inicializar.
 * @param port Puerto GPIO al que se encuentra conectado el LED.
 * @param pin Pin del puerto GPIO correspondiente al LED.
 */
void API_LED_port_Init(led_port_t* led, GPIO_TypeDef *port, uint16_t pin);

/**
 * @brief Enciende fisicamente el LED.
 *
 * Fuerza el pin GPIO asociado al LED al nivel logico correspondiente al estado
 * encendido.
 *
 * @param led Puntero a la estructura del LED a encender.
 */
void API_LED_port_On(led_port_t* led);

/**
 * @brief Apaga fisicamente el LED.
 *
 * Fuerza el pin GPIO asociado al LED al nivel logico correspondiente al estado
 * apagado.
 *
 * @param led Puntero a la estructura del LED a apagar.
 */
void API_LED_port_Off(led_port_t* led);

/**
 * @brief Conmuta fisicamente el estado actual del LED.
 *
 * Invierte el nivel logico presente en el pin GPIO asociado al LED.
 *
 * @param led Puntero a la estructura del LED a conmutar.
 */
void API_LED_port_Toggle(led_port_t* led);

/**
 * @brief Obtiene el estado fisico actual del LED.
 *
 * Lee el nivel logico presente en el pin GPIO asociado al LED y lo devuelve
 * como valor booleano.
 *
 * @param led Puntero al descriptor de hardware del LED a consultar.
 *
 * @return `true` si el LED se encuentra en nivel alto, `false` en caso
 *         contrario.
 */
bool_t API_LED_port_GetValue(led_port_t* led);

#endif /* API_INC_API_LED_PORT_H_ */
