/**
 * @file API_led.h
 * @brief Interfaz publica de la logica de control de LEDs.
 *
 * Este archivo declara la API de alto nivel del modulo de LEDs. La capa
 * `API_led` administra el estado logico de cada LED, su modo de operacion y la
 * temporizacion del parpadeo, delegando el acceso directo al hardware en el
 * modulo `API_led_port`.
 */

#ifndef API_INC_API_LED_H_
#define API_INC_API_LED_H_

/********************************************************/
/* Includes */
#include "stm32f4xx_hal.h"
#include "API_delay.h"
#include "API_led_port.h"

/********************************************************/
/* Declaracion de Tipos Publicos */
/**
 * @brief Modos de funcionamiento disponibles para un LED.
 *
 * Define el comportamiento logico que adopta el LED al ser administrado por la
 * API de alto nivel, permitiendo funcionamiento fijo o intermitente.
 */
typedef enum {
	INVALID = 0, ///<  El led no esta en un estado invalido.
	FIX ,  		///< Mantiene el LED en estado fijo.
	BLINK       ///< Hace parpadear el LED segun el retardo configurado.
} led_mode_e;

/**
 * @brief Estructura de configuracion y estado logico de un LED.
 *
 * Agrupa el descriptor de hardware del LED y la informacion de control que
 * utiliza la capa de logica para gestionar su modo de operacion y la
 * temporizacion del parpadeo.
 */
typedef struct {
	led_port_t hwLed;    ///< Descriptor de hardware administrado por `API_led_port`.
	led_mode_e mode;     ///< Modo de funcionamiento actual del LED.
	delay_t delay;       ///< Retardo utilizado para temporizar el parpadeo.
} led_t;

/********************************************************/
/* Declaracion de Funciones Publicas */
/**
 * @brief Inicializa la logica del LED y su recurso de hardware asociado.
 *
 * Inicializa la estructura de control del LED, delega la configuracion fisica
 * del GPIO al modulo `API_led_port` y deja cargado un parpadeo por defecto.
 *
 * @param led Puntero a la estructura del LED a inicializar.
 * @param port Puerto GPIO al que se encuentra conectado el LED.
 * @param pin Pin del puerto GPIO correspondiente al LED.
 */
void API_LED_Init(led_t* led, GPIO_TypeDef *port, uint16_t pin);

/**
 * @brief Solicita el encendido del LED.
 *
 * Ordena a la capa de port llevar el LED al estado encendido.
 *
 * @param led Puntero a la estructura del LED a encender.
 */
void API_LED_On(led_t* led);

/**
 * @brief Solicita el apagado del LED.
 *
 * Ordena a la capa de port llevar el LED al estado apagado.
 *
 * @param led Puntero a la estructura del LED a apagar.
 */
void API_LED_Off(led_t* led);

/**
 * @brief Solicita la conmutacion del estado actual del LED.
 *
 * Delega en la capa de port la inversion del estado fisico actual del LED.
 *
 * @param led Puntero a la estructura del LED a conmutar.
 */
void API_LED_Toggle(led_t* led);

/**
 * @brief Ejecuta la logica de control periodico del LED.
 *
 * Atiende el comportamiento temporal del LED y, cuando el modo configurado es
 * intermitente, decide cuando debe conmutarse el estado fisico del hardware.
 *
 * @param led Puntero a la estructura del LED a procesar.
 */
void API_LED_Engine(led_t* led);

/**
 * @brief Pausa o reanuda el parpadeo de un LED en modo intermitente.
 *
 * Permite detener temporalmente la conmutacion del LED sin abandonar el modo
 * `BLINK`. Al pausar, la implementacion apaga fisicamente el LED; al reanudar,
 * `API_LED_Engine` vuelve a atender el temporizado configurado.
 *
 * @param led Puntero a la estructura del LED a pausar o reanudar.
 * @param pause `true` para pausar el parpadeo, `false` para reanudarlo.
 */
void API_LED_Pause(led_t* led, bool_t pause);

/**
 * @brief Configura el modo de funcionamiento del LED.
 *
 * Actualiza el modo logico de operacion del LED entre comportamiento fijo o
 * intermitente.
 *
 * @param led Puntero a la estructura del LED a configurar.
 * @param mode Nuevo modo de funcionamiento del LED.
 */
void API_LED_SetMode(led_t* led, led_mode_e mode);

/**
 * @brief Configura la frecuencia de parpadeo del LED.
 *
 * Ajusta el retardo interno utilizado por la logica de parpadeo a partir de la
 * frecuencia indicada en Hertz.
 *
 * @param led Puntero a la estructura del LED a configurar.
 * @param freq Frecuencia de parpadeo deseada en Hertz.
 */
void API_LED_SetBlinkFreq(led_t* led, uint32_t freq);

/**
 * @brief Obtiene el estado actual del LED.
 *
 * Consulta a la capa de port el estado fisico actual del LED asociado a la
 * estructura de control.
 *
 * @param led Puntero a la estructura del LED a consultar.
 *
 * @return `true` si el LED se encuentra encendido, `false` en caso contrario.
 */
bool_t API_LED_GetValue(led_t* led);

/**
 * @brief Obtiene el modo de funcionamiento actual del LED.
 *
 * Devuelve el modo logico actualmente configurado para el LED en la capa de
 * control de alto nivel.
 *
 * @param led Puntero a la estructura del LED a consultar.
 *
 * @return Modo de funcionamiento actual del LED.
 */
led_mode_e API_LED_GetMode(led_t* led);

#endif /* API_INC_API_LED_H_ */
