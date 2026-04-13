/*
 * API_intSensors_port.h
 *
 *  Created on: Apr 12, 2026
 *      Author: joaquin
 */

#ifndef API_INTSENSORS_PORT_H_
#define API_INTSENSORS_PORT_H_

#include "stm32f4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

typedef bool bool_t;

bool_t API_intSensors_port_init(void);
bool_t API_intSensors_port_readTempRaw(uint32_t timeout, uint32_t *value);
bool_t API_intSensors_port_readTempCelsius(uint32_t timeout, float *temperatureC);
bool_t API_intSensors_port_readVoltRaw(uint32_t timeout, uint32_t *value);
bool_t API_intSensors_port_readVoltMilliVolts(uint32_t timeout, uint32_t *voltageMv);
bool_t API_intSensors_port_readAllSensors(uint32_t timeout, float* temperatureC, uint32_t* voltageMv);

#endif /* API_INTSENSORS_PORT_H_ */
