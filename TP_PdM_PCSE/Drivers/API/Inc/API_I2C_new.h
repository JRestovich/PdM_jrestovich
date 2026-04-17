/*
 * API_I2C.h
 *
 *  Created on: 8 abr. 2023
 *      Author: ipave
 */

#ifndef API_INC_API_I2C_H_
#define API_INC_API_I2C_H_

#include <stdio.h>
#include "typedefs.h"

#define I2C_CLOCK_RATE 100000

bool_t API_I2C_DefaultConfig(I2C_HandleTypeDef *hi2c, I2C_TypeDef* i2cInstance);
bool_t API_I2C_Init(I2C_HandleTypeDef *hi2c, uint16_t address);
bool_t API_I2C_Tx(I2C_HandleTypeDef *hi2c, uint16_t address, uint8_t* value);
bool_t API_I2C_Rx(I2C_HandleTypeDef *hi2c, uint16_t address, uint8_t* value);

//HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c,
//                                          uint16_t DevAddress,
//                                          uint8_t *pData,
//                                          uint16_t Size,
//                                          uint32_t Timeout);
//
//HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c,
//                                         uint16_t DevAddress,
//                                         uint8_t *pData,
//                                         uint16_t Size,
//                                         uint32_t Timeout);

_Bool I2C_HW_init(void);

#endif /* API_INC_API_I2C_H_ */
