/*
 * API_MPR121.c
 *
 *  Created on: Apr 19, 2026
 *      Author: joaquin
 */
#include "API_MPR121.h"
#include "API_MPR121_port.h"

static uint16_t lastValue = 0;

bool_t API_MPR121_init() {
	lastValue = 0U;
	return API_MPR121_port_init();
}

bool_t API_MPR121_newValue() {
	uint16_t readKeys = 0;
	if (!API_MPR121_port_readKeys(&readKeys)) {
		return false;
	}
	if (lastValue == readKeys) {
		return false;
	}
	lastValue = readKeys;
	return true;
}

bool_t API_MPR121_getKey(MPR121_keyValue key) {
	return (lastValue & (uint16_t)key) != 0U;
}
