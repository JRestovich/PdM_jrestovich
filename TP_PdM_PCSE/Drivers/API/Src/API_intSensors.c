/*
 * API_intSensors.c
 *
 *  Created on: Apr 12, 2026
 *      Author: joaquin
 */

#include "API_intSensors_port.h"

bool_t API_intSensors_init(void) {
	return API_intSensors_port_init();
}

bool_t API_intSensors_readTempCelsius(uint32_t timeout, float *temperatureC) {
	return API_intSensors_port_readTempCelsius(timeout, temperatureC);
}

bool_t API_intSensors_readVoltMilliVolts(uint32_t timeout, uint32_t *voltageMv) {
	return API_intSensors_port_readVoltMilliVolts(timeout, voltageMv);
}
