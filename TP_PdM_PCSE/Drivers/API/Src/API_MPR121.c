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

/**
 * @brief Checks whether a value has exactly one bit set.
 *
 * @param value Value to evaluate.
 * @return true The value represents exactly one active key.
 * @return false The value has no active keys or multiple active keys.
 */
static bool_t hasSingleBitSet(uint16_t value);

/**
 * @brief Converts the last confirmed keypad value to its numeric digit.
 *
 * Maps the internally stored keypad state to a decimal digit only for keys
 * `0` to `9`. Non-numeric keys such as `*` and `#` are rejected.
 *
 * @param value Pointer to the variable where the detected digit is stored.
 * @return true The stored value maps to a single numeric digit.
 * @return false The stored value does not correspond to a numeric digit.
 */
static bool_t getSingleBitValue(uint8_t *value);

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

bool_t API_MPR121_getSingleKey(uint8_t *key) {
	if (!hasSingleBitSet(actualValue)) {
		return false;
	}

	return getSingleBitValue(key);
}

/***************************************************/
static bool_t hasSingleBitSet(uint16_t value) {
	return (value != RELEASED_VALUE) && ((value & (value - 1U)) == 0U);
}

static bool_t getSingleBitValue(uint8_t *value) {
    bool_t ret = false;
    switch (actualValue) {
        case key_0:
            *value = 0;
            ret = true;
            break;
        case key_1:
            *value = 1;
            ret = true;
            break;
        case key_2:
            *value = 2;
            ret = true;
            break;
        case key_3:
            *value = 3;
            ret = true;
            break;
        case key_4:
            *value = 4;
            ret = true;
            break;
        case key_5:
            *value = 5;
            ret = true;
            break;
        case key_6:
            *value = 6;
            ret = true;
            break;
        case key_7:
            *value = 7;
            ret = true;
            break;
        case key_8:
            *value = 8;
            ret = true;
            break;
        case key_9:
            *value = 9;
            ret = true;
            break;
        default:
            break;
    }

    return ret;
}

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
