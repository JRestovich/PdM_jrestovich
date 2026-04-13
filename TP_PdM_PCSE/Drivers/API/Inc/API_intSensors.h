/*
 * API_intSensors.h
 *
 *  Created on: Apr 12, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_INTSENSORS_H_
#define API_INC_API_INTSENSORS_H_

#include <stdbool.h>

typedef bool bool_t;

bool_t API_intSensors_init(void);
bool_t API_intSensors_readTempCelsius(uint32_t timeout, float *temperatureC);
bool_t API_intSensors_readVoltMilliVolts(uint32_t timeout, uint32_t *voltageMv);

#endif /* API_INC_API_INTSENSORS_H_ */
