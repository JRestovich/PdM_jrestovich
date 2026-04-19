/*
 * API_MPR121.c
 *
 *  Created on: Apr 19, 2026
 *      Author: joaquin
 */
#include "API_MPR121.h"
#include "API_MPR121_port.h"

#define RELEASED_VALUE 0U

static keyboardState state = RELEASED;
static uint16_t lastValue = 0;
static uint16_t actualValue = 0;

bool_t valueChange(uint16_t *newValue);
bool_t updateValue(bool_t update, uint16_t *newValue);

bool_t API_MPR121_init() {
	lastValue = RELEASED_VALUE;
	actualValue = RELEASED_VALUE;
	return API_MPR121_port_init();
}

bool_t API_MPR121_getKey(MPR121_keyValue key) {
	return (actualValue & (uint16_t)key) != 0U;
}

bool_t API_MPR121_readKeys (uint16_t *newValue) {
	bool_t update = false;
	*newValue = RELEASED_VALUE;

	switch (state) {
	case RELEASED:
		if (valueChange(newValue) && *newValue != RELEASED_VALUE) {
			state = PRESSED;
		}
		break;

	case PRESSED:
		if (valueChange(newValue)) {
			if (*newValue != RELEASED_VALUE) {
				lastValue = *newValue;
			} else {
				state = RELEASED;
				update = true;
				actualValue = lastValue;
				lastValue = RELEASED_VALUE;
			}
		}
		break;

	default:
		state = RELEASED;
		break;
	}

	return updateValue(update, newValue);
}

/***************************************************/
bool_t valueChange(uint16_t *newValue) {
	if (!API_MPR121_port_readKeys(newValue)) {
		return false;
	}
	return lastValue != *newValue;
}

bool_t updateValue(bool_t update, uint16_t *newValue) {
	if (!update) {
		*newValue = RELEASED_VALUE;
	}

	return update;
}
