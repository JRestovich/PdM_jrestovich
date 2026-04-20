/**
 * @file API_MPR121.c
 * @brief MPR121 keypad driver implementation.
 *
 * Created on: Apr 19, 2026
 * Author: joaquin
 */
#include "API_MPR121.h"
#include "API_MPR121_port.h"
#include <stdio.h>

#define RELEASED_VALUE 0U ///< Value used when no key is active.

static keyboardState state = RELEASED; ///< Current keypad state.
static uint16_t lastValue = 0; ///< Last raw keypad value read from the device.
static uint16_t actualValue = 0; ///< Last confirmed keypad value reported to the user.

/**
 * @brief Detects whether the keypad value changed since the last reading.
 *
 * @param newValue Pointer to the variable where the raw keypad value is stored.
 * @return true The keypad value changed.
 * @return false The keypad value did not change or the read failed.
 */
bool_t valueChange(uint16_t *newValue);

/**
 * @brief Updates the output value according to the current event state.
 *
 * @param update Indicates whether a new valid value is available.
 * @param newValue Pointer to the variable where the reported value is stored.
 * @return true A new value is available.
 * @return false No new value is available.
 */
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
	} else {
	    *newValue = actualValue;
	}

	return update;
}
