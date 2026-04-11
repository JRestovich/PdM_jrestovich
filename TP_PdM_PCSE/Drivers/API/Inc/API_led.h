/**
 * @file API_led.h
 * @brief Interfaz publica del modulo de control de LEDs.
 *
 * Este archivo declara los tipos de datos y las funciones publicas necesarias
 * para inicializar y controlar LEDs conectados a GPIOs del microcontrolador.
 */

#ifndef API_INC_API_LED_H_
#define API_INC_API_LED_H_

#include "stm32f4xx_hal.h"
#include "API_delay.h"

/**
 * @brief Modos de funcionamiento disponibles para un LED.
 *
 * Define el comportamiento que adopta el LED al ser administrado por el
 * modulo, permitiendo funcionamiento fijo o intermitente.
 */
typedef enum {
	FIX = 0,  ///< Mantiene el LED en estado fijo.
	BLINK     ///< Hace parpadear el LED segun el retardo configurado.
} led_mode_e;

/**
 * @brief Estructura de configuracion y estado de un LED.
 *
 * Agrupa la informacion necesaria para identificar el GPIO asociado al LED,
 * almacenar su modo de operacion y gestionar la temporizacion del parpadeo.
 */
typedef struct {
	GPIO_TypeDef *port;  ///< Puerto GPIO asociado al LED.
	uint16_t pin;        ///< Pin GPIO asociado al LED.
	led_mode_e mode;     ///< Modo de funcionamiento actual del LED.
	delay_t delay;       ///< Retardo utilizado para temporizar el parpadeo.
} led_t;

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
void API_LED_Init(led_t* led, GPIO_TypeDef *port, uint16_t pin);

/**
 * @brief Enciende el LED.
 *
 * Fuerza el pin asociado al LED al estado logico alto.
 *
 * @param led Puntero a la estructura del LED a encender.
 */
void API_LED_On(led_t* led);

/**
 * @brief Apaga el LED.
 *
 * Fuerza el pin asociado al LED al estado logico bajo.
 *
 * @param led Puntero a la estructura del LED a apagar.
 */
void API_LED_Off(led_t* led);

/**
 * @brief Conmuta el estado actual del LED.
 *
 * Invierte el nivel logico presente en el pin asociado al LED.
 *
 * @param led Puntero a la estructura del LED a conmutar.
 */
void API_LED_Toggle(led_t* led);

/**
 * @brief Ejecuta la logica de control periodico del LED.
 *
 * Atiende el comportamiento temporal del LED y realiza el parpadeo cuando el
 * modo configurado es intermitente.
 *
 * @param led Puntero a la estructura del LED a procesar.
 */
void API_LED_Engine(led_t* led);

/**
 * @brief Configura el modo de funcionamiento del LED.
 *
 * Actualiza el modo de operacion del LED entre comportamiento fijo o
 * intermitente.
 *
 * @param led Puntero a la estructura del LED a configurar.
 * @param mode Nuevo modo de funcionamiento del LED.
 */
void API_LED_SetMode(led_t* led, led_mode_e mode);

/**
 * @brief Configura la frecuencia de parpadeo del LED.
 *
 * Ajusta el retardo interno a partir de la frecuencia de parpadeo indicada
 * en Hertz.
 *
 * @param led Puntero a la estructura del LED a configurar.
 * @param freq Frecuencia de parpadeo deseada en Hertz.
 */
void API_LED_SetBlinkFreq(led_t* led, uint32_t freq);

#endif /* API_INC_API_LED_H_ */
