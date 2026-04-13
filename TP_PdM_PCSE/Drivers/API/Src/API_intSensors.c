/**
 * @file API_intSensors.c
 * @brief Implementación de la API de alto nivel para sensores internos.
 *
 * Este archivo implementa la capa de abstracción de aplicación para el acceso a
 * los sensores internos del microcontrolador. La lógica de adquisición y
 * conversión específica del hardware permanece encapsulada en el módulo
 * `API_intSensors_port`.
 */
/********************************************************/
/* Includes                                             */
/********************************************************/

#include "API_intSensors_port.h"

/********************************************************/
/* Implementacion de Funciones Publicas                 */
/********************************************************/

bool_t API_intSensors_init(void) {
	return API_intSensors_port_init();
}

bool_t API_intSensors_readTempCelsius(uint32_t timeout, float *temperatureC) {
	return API_intSensors_port_readTempCelsius(timeout, temperatureC);
}

bool_t API_intSensors_readVoltMilliVolts(uint32_t timeout, uint32_t *voltageMv) {
	return API_intSensors_port_readVoltMilliVolts(timeout, voltageMv);
}

bool_t API_intSensors_readAllSensors(uint32_t timeout,  float *temperatureC, uint32_t *voltageMv) {
	return API_intSensors_port_readAllSensors(timeout, temperatureC, voltageMv);
}
