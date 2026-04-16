/*
 * API_I2C.h
 *
 *  Created on: 8 abr. 2023
 *      Author: ipave
 */

#ifndef API_INC_API_I2C_H_
#define API_INC_API_I2C_H_

#include <stdio.h>
#include "stm32f4xx_hal.h"  		/* <- HAL include */
//#include "stm32f4xx_nucleo_144.h" 	/* <- BSP include */
#include <stdbool.h>

#define I2C_CLOCK_RATE 100000

/*
 * Grupo HAL asociado a las operaciones de comunicacion I2C:
 * "Input and Output operation functions"
 *
 * Rutinas bloqueantes en modo maestro:
 * - HAL_I2C_Master_Transmit(): envia Size bytes al esclavo DevAddress.
 * - HAL_I2C_Master_Receive(): recibe Size bytes desde el esclavo DevAddress.
 *
 * La HAL se encarga de:
 * - Generar START y STOP.
 * - Enviar la direccion del esclavo y el bit R/W.
 * - Transferir los datos byte a byte.
 * - Verificar banderas/errores del periferico.
 * - Respetar el timeout configurado por el usuario.
 *
 * Nota: en STM32 la direccion de 7 bits suele pasarse alineada a la izquierda
 * (por ejemplo, addr << 1), tal como se usa en HAL_I2C_Master_Transmit().
 */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c,
                                          uint16_t DevAddress,
                                          uint8_t *pData,
                                          uint16_t Size,
                                          uint32_t Timeout);

HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c,
                                         uint16_t DevAddress,
                                         uint8_t *pData,
                                         uint16_t Size,
                                         uint32_t Timeout);

_Bool I2C_HW_init(void);

#endif /* API_INC_API_I2C_H_ */
