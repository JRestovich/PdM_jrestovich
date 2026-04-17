/*
 * API_LCD16x2_port.c
 *
 *  Created on: Apr 17, 2026
 *      Author: joaquin
 */
#include "API_LCD16x2_port.h"

#define ROW_QTY 2
#define LCD_DIR 0x27
#define I2C_INSTANCE I2C1

I2C_HandleTypeDef hi2c;

bool_t API_LCD16x2_Init(uint16_t address) {
	if (!API_I2C_DefaultConfig(&hi2c, I2C_INSTANCE)) {
		return false;
	}
	if (!API_I2C_Init(&hi2c, LCD_DIR)) {
		return false;
	}
	return true;
}

bool_t API_LCD16x2_Write_Byte(uint8_t valor) {
	return API_I2C_Tx(&hi2c, LCD_DIR, &valor);
}
