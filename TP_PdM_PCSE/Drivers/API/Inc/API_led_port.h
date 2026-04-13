/**
 * @file API_led_port.h
 * @brief Interfaz publica del modulo de control de LEDs.
 *
 * Este archivo declara los tipos de datos y las funciones publicas necesarias
 * para inicializar y controlar LEDs conectados a GPIOs del microcontrolador.
 */

#ifndef API_INC_API_LED_PORT_H_
#define API_INC_API_LED_PORT_H_

#include "stm32f4xx_hal.h"

/**
 * @brief Estructura de configuracion y estado de un LED.
 *
 * Agrupa la informacion necesaria para identificar el GPIO asociado al LED,
 * almacenar su modo de operacion y gestionar la temporizacion del parpadeo.
 */
typedef struct {
	GPIO_TypeDef *port;  ///< Puerto GPIO asociado al LED.
	uint16_t pin;        ///< Pin GPIO asociado al LED.
} led_port_t;

/**
 * @brief Inicializa la estructura y el hardware asociado a un LED.
 *
 * Configura el pin indicado como salida digital, lo lleva al estado apagado
 * e inicializa los datos internos del modulo para operar el LED.
 *
 * @param led Puntero a la estructura del LED a inicializar.
 * @param port Puerto GPIO al que se encuentra conectado el LED.
 * @param pin Pin del puerto GPIO correspondiente al LED.
 */
void API_LED_port_Init(led_port_t* led, GPIO_TypeDef *port, uint16_t pin);

/**
 * @brief Enciende el LED.
 *
 * Fuerza el pin asociado al LED al estado logico alto.
 *
 * @param led Puntero a la estructura del LED a encender.
 */
void API_LED_port_On(led_port_t* led);

/**
 * @brief Apaga el LED.
 *
 * Fuerza el pin asociado al LED al estado logico bajo.
 *
 * @param led Puntero a la estructura del LED a apagar.
 */
void API_LED_port_Off(led_port_t* led);

/**
 * @brief Conmuta el estado actual del LED.
 *
 * Invierte el nivel logico presente en el pin asociado al LED.
 *
 * @param led Puntero a la estructura del LED a conmutar.
 */
void API_LED_port_Toggle(led_port_t* led);


#endif /* API_INC_API_LED_PORT_H_ */
