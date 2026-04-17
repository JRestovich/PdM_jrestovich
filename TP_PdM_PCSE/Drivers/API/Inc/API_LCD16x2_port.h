/*
 * API_LCD16x2_port.h
 *
 *  Created on: Apr 17, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_LCD16X2_PORT_H_
#define API_INC_API_LCD16X2_PORT_H_

#include <API_I2C_DEVICE.h>

bool_t API_LCD16x2_port_Init(I2C_Device_t* port, uint16_t address, I2C_TypeDef* i2cInstance);
bool_t API_LCD16x2_port_Write_Byte(I2C_Device_t* port, uint8_t valor);
bool_t API_LCD16x2_port_Read_Byte(I2C_Device_t *port, uint8_t *value);
bool_t API_LCD16x2_port_Is_Alive(I2C_Device_t *port);

#endif /* API_INC_API_LCD16X2_PORT_H_ */
