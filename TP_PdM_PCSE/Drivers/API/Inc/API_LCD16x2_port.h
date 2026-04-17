/*
 * API_LCD16x2_port.h
 *
 *  Created on: Apr 17, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_LCD16X2_PORT_H_
#define API_INC_API_LCD16X2_PORT_H_

#include "API_I2C_new.h"

bool_t API_LCD16x2_Init(uint16_t address);
bool_t API_LCD16x2_Write_Byte(uint8_t valor);

#endif /* API_INC_API_LCD16X2_PORT_H_ */
