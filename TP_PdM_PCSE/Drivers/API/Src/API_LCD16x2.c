/**
 * @file API_LCD16x2.c
 * @brief Implementación de la API de alto nivel para el display LCD 16x2.
 *
 * Este archivo implementa la secuencia de inicialización y las operaciones de
 * control y escritura sobre el display LCD. La documentación de la API pública
 * se mantiene en `API_LCD16x2.h`, mientras que aquí se documentan únicamente
 * los elementos privados del módulo.
 */

/********************************************************/
/* Includes */
#include "API_LCD16x2.h"
#include <stdio.h>
#include <string.h>

/********************************************************/
/* Defines */
#define LCD_DIR 0x27U                   ///< Dirección I2C del expansor asociado al LCD.
#define DATOS 1U                        ///< Selección de transferencia de datos hacia el LCD.
#define CONTROL 0U                      ///< Selección de transferencia de comandos hacia el LCD.
#define CLR_LCD 1U                      ///< Comando para limpiar el contenido del display.
#define RETURN_HOME (1U << 1)           ///< Comando para retornar el cursor al origen.
#define ENTRY_MODE (1U << 2)            ///< Comando base de configuración del modo de entrada.
#define DISPLAY_CONTROL (1U << 3)       ///< Comando base de control de display y cursor.
#define CURSOR_DISPLAY_SHIFT (1U << 4)  ///< Comando base de desplazamiento de cursor o display.
#define FUNTION_SET (1U << 5)           ///< Comando de configuración de función del LCD.
#define SET_CGRAM (1U << 6)             ///< Comando para seleccionar la memoria CGRAM.
#define SET_DDRAM (1U << 7)             ///< Comando para seleccionar la memoria DDRAM.
#define DISPLAY_ON (1U << 2)            ///< Máscara para habilitar la visualización del display.
#define SHIFT_RIGHT (1U << 2)           ///< Máscara para desplazar hacia la derecha.
#define DISPLAY_SHIFT (1U << 3)         ///< Máscara para desplazar todo el display.
#define CURSOR_ON (1U << 1)             ///< Máscara para habilitar la visualización del cursor.
#define CURSOR_BLINK 1U                 ///< Máscara para habilitar el parpadeo del cursor.
#define AUTOINCREMENT (1U << 1)         ///< Máscara para incrementar automáticamente la dirección.
#define _4BIT_MODE 0x28U                ///< Comando de inicialización del LCD en modo de 4 bits.
#define DELAY20ms 20U                   ///< Demora inicial de estabilización del display.
#define DELAY10ms 10U                   ///< Demora intermedia utilizada en la secuencia de arranque.
#define DELAY2ms 2U                     ///< Demora requerida por comandos lentos del controlador.
#define DELAY1ms 1U                     ///< Demora corta utilizada durante la inicialización.
#define DelayTime 1U                    ///< Demora entre flancos de enable al enviar nibbles.
#define HIGH_NIBBLE 0xF0U               ///< Máscara para extraer el nibble alto de un byte.
#define LOW_NIBBLE 4U                   ///< Cantidad de bits para desplazar entre nibbles.
#define LINEA1 0x80U                    ///< Dirección DDRAM base correspondiente a la primera fila.
#define LINEA2 0xC0U                    ///< Dirección DDRAM base correspondiente a la segunda fila.
#define COMANDO_INI1 0x30U              ///< Primer comando repetido de arranque en modo 8 bits.
#define COMANDO_INI2 0x20U              ///< Comando de transición desde 8 bits hacia modo 4 bits.

/********************************************************/
/* Variables privadas */
static const char blankDisplay[] = "                ";  ///< Cadena de 16 espacios usada para efectos de entrada.
static const uint8_t LCD_INIT_CMD[] = {                 ///< Secuencia de comandos estándar de inicialización del LCD.
	_4BIT_MODE,
	DISPLAY_CONTROL + DISPLAY_ON,
	CLR_LCD,
	ENTRY_MODE + AUTOINCREMENT,
	RETURN_HOME
};

static I2C_Device_t lcdPort;                      ///< Descriptor I2C privado asociado al display LCD.
static uint8_t displayControlFlags = DISPLAY_ON; ///< Banderas actuales de visualización y cursor.
static uint8_t entryModeFlags = AUTOINCREMENT;   ///< Banderas actuales del modo de entrada del LCD.
static bool_t backlightOn = true;                ///< Estado actual de la retroiluminación del display.

/********************************************************/
/* Declaracion de funciones privadas */
/**
 * @brief Envía un nibble al LCD mediante el expansor I2C.
 *
 * Construye el byte de control correspondiente, conmuta la línea enable y
 * transmite el nibble indicado como dato o comando.
 *
 * @param valor Nibble alineado en la mitad alta del byte.
 * @param tipo Tipo de transferencia: dato o comando.
 */
static void send4bit(uint8_t valor, bool_t tipo);

/**
 * @brief Envía un byte completo al LCD en dos transferencias de nibble.
 *
 * @param valor Byte completo a transmitir.
 * @param tipo Tipo de transferencia: dato o comando.
 */
static void send8bit(uint8_t valor, bool_t tipo);

/**
 * @brief Envía un comando de control al LCD.
 *
 * @param valor Comando a transmitir al controlador del display.
 */
static void controlLcd(uint8_t valor);

/**
 * @brief Reescribe en el LCD el estado actual de las banderas de visualización.
 *
 * Emite el comando de control de display utilizando el valor almacenado en las
 * banderas privadas del módulo.
 */
static void writeDisplayControl(void);

/********************************************************/
/* Implementacion de funciones publicas */
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

void API_LCD16x2_SendString(const char *data, uint16_t size) {
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

void API_LCD16x2_WriteStringAt(uint8_t row, uint8_t col, const char *data, uint16_t size) {
	if (data == NULL) {
		return;
	}

	API_LCD16x2_SetCursor(row, col);
	API_LCD16x2_SendString(data, size);
}

void API_LCD16x2_UpdateFirstRow(const char *data, uint16_t size) {
	if (data == NULL) {
		return;
	}

	API_LCD16x2_WriteStringAt(0, 0, blankDisplay, strlen(blankDisplay));
	API_LCD16x2_WriteStringAt(0, 0, data, size);
}

void API_LCD16x2_UpdateSecondRow(const char *data, uint16_t size) {
	if (data == NULL) {
		return;
	}

	API_LCD16x2_WriteStringAt(1, 0, blankDisplay, strlen(blankDisplay));
	API_LCD16x2_WriteStringAt(1, 0, data, size);
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

void API_LCD16x2_LoadTextFromRight(const char *data, uint16_t size) {
	if (data == NULL) {
		return;
	}

	char buffer[17 + size];
	snprintf(buffer, sizeof(buffer), "%s%.*s", blankDisplay, size, data);

	API_LCD16x2_SendString(buffer, strlen(buffer));
}

void API_LCD16x2_sendSingleNumber(uint8_t number) {
    API_LCD16x2_SendByte('0' + number);
}

/********************************************************/
/* Implementacion de funciones privadas */
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
