/*
 * API_LCD16x2.c
 *
 *  Created on: Apr 17, 2026
 *      Author: joaquin
 */

#include "API_LCD16x2.h"
#include "API_LCD16x2_port.h"
#include "API_delay.h"
#include <stdio.h>
#include <string.h>

#define LCD_DIR 0x27

#define DATOS 1
#define CONTROL 0

#define CLR_LCD 1
#define RETURN_HOME (1<<1)
#define ENTRY_MODE (1<<2)
#define DISPLAY_CONTROL (1<<3)
#define CURSOR_DISPLAY_SHIFT (1<<4)
#define FUNTION_SET (1<<5)
#define SET_CGRAM (1<<6)
#define SET_DDRAM (1<<7)
/************************************/
#define DISPLAY_ON (1<<2)
#define SHIFT_RIGHT (1<<2)
#define DISPLAY_SHIFT (1<<3)
#define CURSOR_ON (1<<1)
#define CURSOR_BLINK 1
#define AUTOINCREMENT (1<<1)
#define _4BIT_MODE 0x28

//DEMORAS NECESARIAS

#define DELAY20ms 20
#define DELAY10ms 10
#define DELAY2ms  2
#define DELAY1ms  1
#define DelayTime 1

//MANEJO DE NIBBLES
#define HIGH_NIBBLE 0xf0
#define LOW_NIBBLE 4

//LINEAS DEL LCD
#define LINEA1 0x80
#define LINEA2 0xc0

//COMANDOS DE INICIALIZACIÓN
#define COMANDO_INI1 0x30
#define COMANDO_INI2 0x20

static const char blankDisplay[] = "                "; // 16 spaces
static const uint8_t LCD_INIT_CMD[]={ _4BIT_MODE,
                                      DISPLAY_CONTROL+DISPLAY_ON,
                                      CLR_LCD,
                                      ENTRY_MODE+AUTOINCREMENT,
                                      RETURN_HOME };

static void send4bit(uint8_t valor, bool_t tipo);
static void send8bit(uint8_t valor, bool_t tipo);
static void controlLcd(uint8_t valor);
static void writeDisplayControl(void);

static I2C_Device_t lcdPort;
static uint8_t displayControlFlags = DISPLAY_ON;
static uint8_t entryModeFlags = AUTOINCREMENT;
static bool_t backlightOn = true;

bool_t API_LCD16x2_Init(void) {

	if (!API_LCD16x2_port_Init(&lcdPort, LCD_DIR, I2C2)) {
	   return false;
	}

	HAL_Delay(DELAY20ms);
	send4bit(COMANDO_INI1, CONTROL);
    HAL_Delay(DELAY10ms);
    send4bit(COMANDO_INI1, CONTROL);
    HAL_Delay(DELAY1ms);
    send4bit(COMANDO_INI1, CONTROL);
	send4bit(COMANDO_INI2, CONTROL);
    for(uint8_t i=0; i < sizeof(LCD_INIT_CMD); i++) {
    	controlLcd(LCD_INIT_CMD[i]);
    }
    displayControlFlags = DISPLAY_ON;
    entryModeFlags = AUTOINCREMENT;
    backlightOn = true;
    HAL_Delay(DELAY2ms);
    return true;
}

void API_LCD16x2_SendByte(char byte) {
	send8bit((uint8_t) byte, DATOS);
}

void API_LCD16x2_SendString(const char* data, uint16_t size) {
	if (data == NULL) {
		return;
	}
	for (uint16_t i = 0; i < size; i++) {
		API_LCD16x2_SendByte(data[i]);
	}
}

void API_LCD16x2_Clear(void) {
	controlLcd(CLR_LCD);
	HAL_Delay(DELAY2ms);
}

void API_LCD16x2_Home(void) {
	controlLcd(RETURN_HOME);
	HAL_Delay(DELAY2ms);
}

void API_LCD16x2_SetDisplayVisible(bool_t visible) {
	if (visible) {
		displayControlFlags |= DISPLAY_ON;
	} else {
		displayControlFlags &= (uint8_t) ~DISPLAY_ON;
	}

	writeDisplayControl();
}

void API_LCD16x2_SetCursorVisible(bool_t visible) {
	if (visible) {
		displayControlFlags |= CURSOR_ON;
	} else {
		displayControlFlags &= (uint8_t) ~CURSOR_ON;
	}

	writeDisplayControl();
}

void API_LCD16x2_SetCursorBlink(bool_t enable) {
	if (enable) {
		displayControlFlags |= CURSOR_BLINK;
	} else {
		displayControlFlags &= (uint8_t) ~CURSOR_BLINK;
	}

	writeDisplayControl();
}

void API_LCD16x2_SetCursor(uint8_t row, uint8_t col) {
	if (row > 1U || col > 15U) {
		return;
	}

	if (row == 0U) {
		API_LCD16x2_FirstRow(col);
		return;
	}

	API_LCD16x2_SecondRow(col);
}

void API_LCD16x2_FirstRow(uint8_t pos) {
	controlLcd(pos | LINEA1);
}

void API_LCD16x2_SecondRow(uint8_t pos) {
	controlLcd(pos | LINEA2);
}

void API_LCD16x2_WriteCharAt(uint8_t row, uint8_t col, char data) {
	API_LCD16x2_SetCursor(row, col);
	API_LCD16x2_SendByte(data);
}

void API_LCD16x2_WriteStringAt(uint8_t row, uint8_t col, const char* data, uint16_t size) {
	if (data == NULL) {
		return;
	}

	API_LCD16x2_SetCursor(row, col);
	API_LCD16x2_SendString(data, size);
}

void API_LCD16x2_ShiftCursor(bool_t right) {
	uint8_t shiftCmd = CURSOR_DISPLAY_SHIFT;

	if (right) {
		shiftCmd |= SHIFT_RIGHT;
	}

	controlLcd(shiftCmd);
}

void API_LCD16x2_ShiftDisplay(bool_t right) {
	uint8_t shiftCmd = CURSOR_DISPLAY_SHIFT | DISPLAY_SHIFT;

	if (right) {
		shiftCmd |= SHIFT_RIGHT;
	}

	controlLcd(shiftCmd);
}

void API_LCD16x2_SetEntryMode(bool_t increment, bool_t shiftDisplay) {
	entryModeFlags = 0U;

	if (increment) {
		entryModeFlags |= AUTOINCREMENT;
	}

	if (shiftDisplay) {
		entryModeFlags |= 1U;
	}

	controlLcd(ENTRY_MODE | entryModeFlags);
}

void API_LCD16x2_Backlight(bool_t on) {
	backlightOn = on;
	controlLcd(DISPLAY_CONTROL | displayControlFlags);
}

void API_LCD16x2_LoadTextFromRight(const char* data, uint16_t size) {
	if (data == NULL) {
		return;
	}

	char buffer[17 + size];
	snprintf(buffer, sizeof(buffer), "%s%.*s", blankDisplay, size, data);

	API_LCD16x2_SendString(buffer, strlen(buffer));
}

/**********************************/

static void controlLcd(uint8_t valor) {
	send8bit(valor, CONTROL);
}

static void writeDisplayControl(void) {
	controlLcd(DISPLAY_CONTROL | displayControlFlags);
}

static void send8bit(uint8_t valor, bool_t tipo) {
	send4bit(valor & HIGH_NIBBLE, tipo);
	send4bit(valor << LOW_NIBBLE, tipo);
}

static void send4bit(uint8_t valor, bool_t tipo) {
	LCD_data_u lcdData = {0};

	/* El nibble llega alineado en bits 7..4, como en API_lcd.c; se desplaza
	 * para cargar el campo de 4 bits y que dataRaw lo reubique al nibble alto.
	 */
	lcdData.data.data = valor >> LOW_NIBBLE;
	lcdData.data.bakLight = backlightOn ? 1U : 0U;
	lcdData.data.enable = 1U;
	lcdData.data.RS = tipo;
	API_LCD16x2_port_Write_Byte(&lcdPort, lcdData.dataRaw);
	HAL_Delay(DelayTime);

	lcdData.data.enable = 0U;
	API_LCD16x2_port_Write_Byte(&lcdPort, lcdData.dataRaw);
	HAL_Delay(DelayTime);
}
