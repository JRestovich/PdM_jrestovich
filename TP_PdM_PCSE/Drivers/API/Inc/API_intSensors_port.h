/**
 * @file API_intSensors_port.h
 * @brief Interfaz de port para la lectura de sensores internos mediante ADC.
 *
 * Este módulo concentra el acceso dependiente del hardware necesario para
 * adquirir las señales de temperatura interna y tensión de referencia interna
 * del microcontrolador. Además de las lecturas en crudo, ofrece funciones que
 * entregan los resultados convertidos a unidades de ingeniería.
 */

#ifndef API_INTSENSORS_PORT_H_
#define API_INTSENSORS_PORT_H_

/********************************************************/
/* Includes */
#include "typedefs.h"

/********************************************************/
/* Declaracion de Funciones Publicas */
/**
 * @brief Inicializa el periférico ADC y la secuencia de canales internos.
 *
 * Habilita el reloj del ADC y configura el periférico para convertir los
 * canales correspondientes al sensor de temperatura interno y a la referencia
 * interna de tensión.
 *
 * @return `true` si la inicialización se realizó correctamente, `false` en caso
 *         contrario.
 */
bool_t API_intSensors_port_init(void);

/**
 * @brief Lee en crudo el valor ADC asociado al sensor de temperatura interno.
 *
 * Ejecuta una secuencia de conversión y devuelve el valor digital sin convertir
 * correspondiente al canal de temperatura.
 *
 * @param timeout Tiempo máximo de espera, en milisegundos, para completar la
 *                conversión ADC.
 * @param value Puntero donde se almacenará el valor ADC leído.
 *
 * @return `true` si la lectura se completó correctamente, `false` si ocurrió un
 *         error o los parámetros son inválidos.
 */
bool_t API_intSensors_port_readTempRaw(uint32_t timeout, uint32_t *value);

/**
 * @brief Lee la temperatura interna del microcontrolador en grados Celsius.
 *
 * Obtiene el valor ADC del sensor de temperatura interno y lo convierte a
 * grados Celsius.
 *
 * @param timeout Tiempo máximo de espera, en milisegundos, para completar la
 *                conversión ADC.
 * @param temperatureC Puntero donde se almacenará la temperatura convertida.
 *
 * @return `true` si la lectura y la conversión se realizaron correctamente,
 *         `false` en caso contrario.
 */
bool_t API_intSensors_port_readTempCelsius(uint32_t timeout, float *temperatureC);

/**
 * @brief Lee en crudo el valor ADC asociado a la referencia interna de tensión.
 *
 * Ejecuta una secuencia de conversión y devuelve el valor digital sin convertir
 * correspondiente al canal de referencia interna.
 *
 * @param timeout Tiempo máximo de espera, en milisegundos, para completar la
 *                conversión ADC.
 * @param value Puntero donde se almacenará el valor ADC leído.
 *
 * @return `true` si la lectura se completó correctamente, `false` si ocurrió un
 *         error o los parámetros son inválidos.
 */
bool_t API_intSensors_port_readVoltRaw(uint32_t timeout, uint32_t *value);

/**
 * @brief Lee la referencia interna de tensión en milivoltios.
 *
 * Obtiene el valor ADC del canal VREFINT y lo convierte a milivoltios.
 *
 * @param timeout Tiempo máximo de espera, en milisegundos, para completar la
 *                conversión ADC.
 * @param voltageMv Puntero donde se almacenará la tensión convertida en
 *                  milivoltios.
 *
 * @return `true` si la lectura y la conversión se realizaron correctamente,
 *         `false` en caso contrario.
 */
bool_t API_intSensors_port_readVoltMilliVolts(uint32_t timeout, uint32_t *voltageMv);

/**
 * @brief Lee en una sola secuencia la temperatura interna y la referencia
 *        interna de tensión.
 *
 * Ejecuta las conversiones ADC de ambos canales internos y devuelve los
 * resultados convertidos a grados Celsius y milivoltios.
 *
 * @param timeout Tiempo máximo de espera, en milisegundos, para completar la
 *                secuencia de conversiones ADC.
 * @param temperatureC Puntero donde se almacenará la temperatura convertida.
 * @param voltageMv Puntero donde se almacenará la tensión convertida en
 *                  milivoltios.
 *
 * @return `true` si ambas lecturas se realizaron correctamente, `false` en caso
 *         contrario.
 */
bool_t API_intSensors_port_readAllSensors(uint32_t timeout, float* temperatureC, uint32_t* voltageMv);

#endif /* API_INTSENSORS_PORT_H_ */
