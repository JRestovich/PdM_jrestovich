/**
 * @file API_intSensors.h
 * @brief Interfaz de alto nivel para la lectura de sensores internos del microcontrolador.
 *
 * Este módulo expone una API simple para inicializar la adquisición de los
 * sensores internos y obtener mediciones de temperatura y tensión de referencia
 * interna ya convertidas a unidades de aplicación. La implementación delega el
 * acceso específico al hardware al módulo de port asociado.
 */

#ifndef API_INC_API_INTSENSORS_H_
#define API_INC_API_INTSENSORS_H_

/********************************************************/
/* Includes */
#include "typedefs.h"

/********************************************************/
/* Declaracion de Funciones Publicas */
/**
 * @brief Inicializa el módulo de sensores internos.
 *
 * Configura los recursos necesarios para habilitar la lectura de los sensores
 * internos del microcontrolador a través de la capa de port.
 *
 * @return `true` si la inicialización se realizó correctamente, `false` en caso
 *         contrario.
 */
bool_t API_intSensors_init(void);

/**
 * @brief Lee la temperatura interna del microcontrolador en grados Celsius.
 *
 * Ejecuta una conversión ADC a través de la capa de port y entrega el valor de
 * temperatura convertido a grados Celsius.
 *
 * @param timeout Tiempo máximo de espera, en milisegundos, para completar la
 *                conversión ADC.
 * @param temperatureC Puntero donde se almacenará la temperatura leída en
 *                     grados Celsius.
 *
 * @return `true` si la lectura se completó correctamente, `false` si ocurrió un
 *         error o los parámetros son inválidos.
 */
bool_t API_intSensors_readTempCelsius(uint32_t timeout, float *temperatureC);

/**
 * @brief Lee la tensión de referencia interna en milivoltios.
 *
 * Ejecuta una conversión ADC a través de la capa de port y devuelve el valor de
 * la referencia interna convertido a milivoltios.
 *
 * @param timeout Tiempo máximo de espera, en milisegundos, para completar la
 *                conversión ADC.
 * @param voltageMv Puntero donde se almacenará la tensión leída en
 *                  milivoltios.
 *
 * @return `true` si la lectura se completó correctamente, `false` si ocurrió un
 *         error o los parámetros son inválidos.
 */
bool_t API_intSensors_readVoltMilliVolts(uint32_t timeout, uint32_t *voltageMv);

/**
 * @brief Lee en una única secuencia la temperatura interna y la tensión de
 *        referencia interna.
 *
 * Solicita ambas conversiones ADC y entrega los resultados ya convertidos a
 * grados Celsius y milivoltios, respectivamente.
 *
 * @param timeout Tiempo máximo de espera, en milisegundos, para completar la
 *                secuencia de conversiones ADC.
 * @param temperatureC Puntero donde se almacenará la temperatura en grados
 *                     Celsius.
 * @param voltageMv Puntero donde se almacenará la tensión de referencia interna
 *                  en milivoltios.
 *
 * @return `true` si ambas lecturas se completaron correctamente, `false` si
 *         ocurrió un error o los parámetros son inválidos.
 */
bool_t API_intSensors_readAllSensors(uint32_t timeout,  float *temperatureC, uint32_t *voltageMv);

#endif /* API_INC_API_INTSENSORS_H_ */
