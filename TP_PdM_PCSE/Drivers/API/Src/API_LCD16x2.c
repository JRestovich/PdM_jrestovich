/*
 * API_LCD16x2.c
 *
 *  Created on: Apr 17, 2026
 *      Author: joaquin
 */

#include "API_lcd.h"
#include "API_LCD16x2_port.h"
#include "API_delay.h"

static const uint8_t LCD_INIT_CMD[]={ _4BIT_MODE,
                                      DISPLAY_CONTROL+DISPLAY_ON,
                                      CLR_LCD,
                                      ENTRY_MODE+AUTOINCREMENT,
                                      RETURN_HOME };

static void sendToLCD (uint8_t valor, bool_t tipo);

static I2C_Device_t lcdPort;

bool_t API_LCD16x2_Init(void) {

	if (!API_LCD16x2_port_Init(&lcdPort, LCD_DIR, I2C2)) {
	   return false;
	}

	HAL_Delay(DELAY20ms);
	sendToLCD(COMANDO_INI1, CONTROL);
    HAL_Delay(DELAY10ms);
    sendToLCD(COMANDO_INI1, CONTROL);
    HAL_Delay(DELAY1ms);
    sendToLCD(COMANDO_INI1, CONTROL);
    sendToLCD(COMANDO_INI2, CONTROL);
    for(uint8_t i=0; i < sizeof(LCD_INIT_CMD); i++) {
    	ControlLcd(LCD_INIT_CMD[i]);
    }
    HAL_Delay(DELAY2ms);
    return true;
}

/**********************************/

static void sendToLCD (uint8_t valor, bool_t tipo) {
	API_LCD16x2_port_Write_Byte(&lcdPort, valor+tipo+EN+BL);
	HAL_Delay(DelayTime);
	API_LCD16x2_port_Write_Byte(&lcdPort, valor+tipo+BL);
	HAL_Delay(DelayTime);
}
