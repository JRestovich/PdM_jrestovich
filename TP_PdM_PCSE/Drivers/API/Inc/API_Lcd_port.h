/*
 * API_LCDport.h
 *
 *  Created on: 25 mar. 2023
 *      Author: ipave
 */

#ifndef API_INC_API_LCD_PORT_H_
#define API_INC_API_LCD_PORT_H_

#include <stdio.h>
#include "API_lcd.h"
#include "typedefs.h"

extern void Error_Handler(void);

bool_t LCD_HW_init(void);
bool_t LCD_Read_Port(uint8_t *port_value);
void LCD_Write_Byte(uint8_t valor);
void GPIO_I2C(I2C_HandleTypeDef *hi2c);

#endif /* API_INC_API_LCD_PORT_H_ */


