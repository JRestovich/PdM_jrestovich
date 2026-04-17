/*
 * API_LCD16x2_port.h
 *
 *  Created on: Apr 17, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_LCD16X2_PORT_H_
#define API_INC_API_LCD16X2_PORT_H_

#include "API_I2C_new.h"

typedef struct {
	I2C_HandleTypeDef hi2c;
	uint16_t address;
} LCD16x2_port;

bool_t API_LCD16x2_port_Init(LCD16x2_port* port, uint16_t address, I2C_TypeDef* i2cInstance);
bool_t API_LCD16x2_port_Write_Byte(LCD16x2_port* port, uint8_t valor);
bool_t API_LCD16x2_port_Read_Byte(LCD16x2_port *port, uint8_t *value);
bool_t API_LCD16x2_port_Is_Alive(LCD16x2_port *port);

#endif /* API_INC_API_LCD16X2_PORT_H_ */
