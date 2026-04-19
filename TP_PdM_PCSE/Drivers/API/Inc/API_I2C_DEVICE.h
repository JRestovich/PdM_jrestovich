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
bool_t API_I2C_DEVICE_MemRead(I2C_Device_t *device,
		uint16_t registerAddress,
		uint16_t registerAddressSize,
		uint8_t *values,
		uint16_t size);
bool_t API_I2C_DEVICE_SetAddress(I2C_Device_t *device, uint16_t address);
bool_t API_I2C_DEVICE_GetAddress(const I2C_Device_t *device, uint16_t *address);

bool_t API_I2C_DEVICE_SetInstance(I2C_Device_t *device, I2C_TypeDef *i2cInstance);
bool_t API_I2C_DEVICE_GetInstance(const I2C_Device_t *device, I2C_TypeDef **i2cInstance);

bool_t API_I2C_DEVICE_SetClockSpeed(I2C_Device_t *device, uint32_t clockSpeed);
bool_t API_I2C_DEVICE_GetClockSpeed(const I2C_Device_t *device, uint32_t *clockSpeed);

bool_t API_I2C_DEVICE_SetDutyCycle(I2C_Device_t *device, uint32_t dutyCycle);
bool_t API_I2C_DEVICE_GetDutyCycle(const I2C_Device_t *device, uint32_t *dutyCycle);

bool_t API_I2C_DEVICE_SetOwnAddress1(I2C_Device_t *device, uint32_t ownAddress1);
bool_t API_I2C_DEVICE_GetOwnAddress1(const I2C_Device_t *device, uint32_t *ownAddress1);

bool_t API_I2C_DEVICE_SetAddressingMode(I2C_Device_t *device, uint32_t addressingMode);
bool_t API_I2C_DEVICE_GetAddressingMode(const I2C_Device_t *device, uint32_t *addressingMode);

bool_t API_I2C_DEVICE_SetDualAddressMode(I2C_Device_t *device, uint32_t dualAddressMode);
bool_t API_I2C_DEVICE_GetDualAddressMode(const I2C_Device_t *device, uint32_t *dualAddressMode);

bool_t API_I2C_DEVICE_SetOwnAddress2(I2C_Device_t *device, uint32_t ownAddress2);
bool_t API_I2C_DEVICE_GetOwnAddress2(const I2C_Device_t *device, uint32_t *ownAddress2);

bool_t API_I2C_DEVICE_SetGeneralCallMode(I2C_Device_t *device, uint32_t generalCallMode);
bool_t API_I2C_DEVICE_GetGeneralCallMode(const I2C_Device_t *device, uint32_t *generalCallMode);

bool_t API_I2C_DEVICE_SetNoStretchMode(I2C_Device_t *device, uint32_t noStretchMode);
bool_t API_I2C_DEVICE_GetNoStretchMode(const I2C_Device_t *device, uint32_t *noStretchMode);

#endif /* API_INC_API_I2C_DEVICE_H_ */
