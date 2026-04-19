/*
 * API_MPR121_port.h
 *
 *  Created on: Apr 13, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_MPR121_PORT_H_
#define API_INC_API_MPR121_PORT_H_

#include "typedefs.h"

bool_t API_MPR121_port_init();
bool_t API_MPR121_port_readKeys(uint16_t *pdata);

#endif /* API_INC_API_MPR121_PORT_H_ */
