/**
 * @file API_MPR121.h
 * @brief Public interface for the MPR121 keypad driver.
 *
 * Created on: Apr 19, 2026
 * Author: joaquin
 */

#ifndef API_INC_API_MPR121_H_
#define API_INC_API_MPR121_H_

/********************************************************/
/* Includes */
#include "API_MPR121_port.h"

/********************************************************/
/* Declaracion de Tipos Publicos */
/**
 * @brief Bit mask for the keypad keys reported by the MPR121.
 */
typedef enum {
	key_1 = 1,
	key_4 = (1<<1),
	key_7 = (1<<2),
	key_asterisk = (1<<3),
	key_2 = (1<<4),
	key_5 = (1<<5),
	key_8 = (1<<6),
	key_0 = (1<<7),
	key_3 = (1<<8),
	key_6 = (1<<9),
	key_9 = (1<<10),
	key_hashtag = (1<<11)
} MPR121_keyValue;

/**
 * @brief Keypad state machine states.
 */
typedef enum {
	RELEASED = 0,
	PRESSED
} keyboardState;

/********************************************************/
/* Declaracion de Funciones Publicas */
/**
 * @brief Initializes the MPR121 driver.
 *
 * @return true Initialization completed successfully.
 * @return false Initialization failed.
 */
bool_t API_MPR121_init();

/**
 * @brief Checks whether a key is active in the last valid reading.
 *
 * @param key Key mask to evaluate.
 * @return true The key is active.
 * @return false The key is not active.
 */
bool_t API_MPR121_getKey(MPR121_keyValue key);

/**
 * @brief Reads the keypad and updates the reported value when a key is released.
 *
 * @param newValue Pointer to the variable where the detected value is stored.
 * @return true A new value is available.
 * @return false No new value is available.
 */
bool_t API_MPR121_readKeys (uint16_t *newValue);

/**
 * @brief Returns the numeric value of the last confirmed key if it is unique.
 *
 * Evaluates the last valid key captured by the driver and converts it to a
 * decimal digit from 0 to 9. The function only succeeds when exactly one key
 * is active and that key corresponds to a numeric keypad key.
 *
 * @param key Pointer to the variable where the detected digit is stored.
 * @return true A single numeric key was detected and converted successfully.
 * @return false No key is available, multiple keys are active, or the key does
 * not map to a numeric digit.
 */
bool_t API_MPR121_getSingleKey(uint8_t *key);

#endif /* API_INC_API_MPR121_H_ */
