/*
 * API_MPR121.h
 *
 *  Created on: Apr 19, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_MPR121_H_
#define API_INC_API_MPR121_H_

#include "API_MPR121_port.h"

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

bool_t API_MPR121_init();
bool_t API_MPR121_newValue();
bool_t API_MPR121_getKey(MPR121_keyValue key);

#endif /* API_INC_API_MPR121_H_ */
