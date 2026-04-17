/*
 * API_I2C_DEVICE.h
 *
 *  Created on: 8 abr. 2023
 *      Author: ipave
 */

#ifndef API_INC_API_I2C_DEVICE_H_
#define API_INC_API_I2C_DEVICE_H_

#include <stdio.h>
#include "typedefs.h"

typedef struct {
    I2C_HandleTypeDef hi2c;
    uint16_t deviceAddress;
} I2C_Device_t;

bool_t API_I2C_DEVICE_DefaultConfig(I2C_Device_t * device, I2C_TypeDef* i2cInstance);
bool_t API_I2C_DEVICE_Init(I2C_Device_t * device, uint16_t address);
bool_t API_I2C_DEVICE_Tx(I2C_Device_t * device, uint16_t address, uint8_t *value, uint16_t size);
bool_t API_I2C_DEVICE_Rx(I2C_Device_t * device, uint16_t address, uint8_t* value);
void API_I2C_DEVICE_SetAddress(I2C_Device_t * device, uint16_t address);

#endif /* API_INC_API_I2C_DEVICE_H_ */
