/*
 * API_MPR121_port.h
 *
 *  Created on: Apr 13, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_MPR121_PORT_H_
#define API_INC_API_MPR121_PORT_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

typedef bool bool_t;

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

bool_t API_MPR121_port_init();
bool_t API_MPR121_port_readKeys(uint16_t *pdata);
bool_t API_MPR121_port_getKeyValue(MPR121_keyValue key);

#endif /* API_INC_API_MPR121_PORT_H_ */
