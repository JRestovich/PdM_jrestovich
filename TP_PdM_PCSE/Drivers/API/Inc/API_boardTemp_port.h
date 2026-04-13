/*
 * API_boardTemp_port.h
 *
 *  Created on: Apr 12, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_BOARDTEMP_PORT_H_
#define API_INC_API_BOARDTEMP_PORT_H_

#include "stm32f4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

typedef bool bool_t;

bool_t API_boardTemp_port_init(void);
bool_t API_boardTemp_port_startConvertion(void);
bool_t API_boardTemp_port_convertionReady(uint32_t timeout);
bool_t API_boardTemp_port_readRaw(uint32_t timeout, uint32_t *value);
bool_t API_boardTemp_port_readCelsius(uint32_t timeout, float *temperatureC);

#endif /* API_INC_API_BOARDTEMP_PORT_H_ */
