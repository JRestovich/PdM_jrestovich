/**
 * @file API_cmdparser.c
 * @brief Implementacion del parser de comandos por UART.
 *
 * El modulo recibe caracteres individuales por UART, arma una linea de
 * comando en un buffer interno y la procesa mediante una maquina de
 * estados. Los comandos validos permiten consultar ayuda y controlar el
 * LED de usuario de la placa.
 */

#include "API_cmdparser.h"
#include "API_uart.h"
#include <string.h>
#include "stm32f4xx_hal.h"

#define BUFFER_SIZE 256
#define LOWER_TO_UPPER 32

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

static const char helpCmd[] = "HELP";
static const char ledOnCmd[] = "LED ON";
static const char ledOffCmd[] = "LED OFF";
static const char ledToggleCmd[] = "LED TOGGLE";
static const char statusCmd[] = "STATUS";
static const char ledOnTx[] = "LED is ON";
static const char ledOffTx[] = "LED is OFF";

static cmdParser_state_t state;
static cmd_status_t status = CMD_OK;

static uint8_t buffer[BUFFER_SIZE];
static uint8_t idx = 0;

static bool_t cmdProcessLine(void);
static void strToUpper(uint8_t *str);

void cmdParserInit(void) {
	if (uartInit()) {
		state = CMD_IDLE;
	} else {
		state = CMD_ERROR;
	}
}

void cmdPoll(void) {
	uint8_t rx;
	uartReceiveStringSize(&rx, 1);

	switch (state) {
	case CMD_IDLE:
		// Return there isn't a new data
		if (!uartReceiveOk()) {
			return;
		}

		if (rx != '\n' && rx != '\r') {
			idx = 0;
			buffer[idx] = rx;
			state = CMD_RECEIVING;
		}
		break;

	case CMD_RECEIVING:
		// Return there isn't a new data
		if (!uartReceiveOk()) {
			return;
		}
		idx++;
		if (idx >= BUFFER_SIZE) {
			state = CMD_ERROR;
			status = CMD_ERR_OVERFLOW;
		} else if (rx == '\n' || rx == '\r') {
			state = CMD_PROCESS;
			buffer[idx] = '\0';
		} else {
			buffer[idx] = rx;
		}
		break;

	case CMD_PROCESS:
		if (!cmdProcessLine()) {
			state = CMD_ERROR;
			status = CMD_OK;
		} else {
			state = CMD_EXEC;
		}
		break;

	case CMD_EXEC:
		if (strcmp((char *)buffer, helpCmd) == 0) {
			cmdPrintHelp();
		} else if (strcmp((char *)buffer, ledOnCmd) == 0) {
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		} else if (strcmp((char *)buffer, ledOffCmd) == 0) {
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		} else if (strcmp((char *)buffer, ledToggleCmd) == 0) {
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		} else if (strcmp((char *)buffer, statusCmd) == 0) {
			if(HAL_GPIO_ReadPin(LD2_GPIO_Port, LD2_Pin) == GPIO_PIN_RESET) {
				uartSendString((uint8_t *)ledOffTx);
			} else {
				uartSendString((uint8_t *)ledOnTx);
			}
			uartSendString((uint8_t *)"\r\n");
		} else {
			state = CMD_ERROR;
			status = CMD_ERR_ARG;
		}
		state = CMD_IDLE;
		break;

	case CMD_ERROR:
		// print error msg
		state = CMD_IDLE;
		break;

	default:
		break;
	}
}

void cmdPrintHelp(void) {
	uartSendString((uint8_t *)"Available commands:\r\n");
	uartSendString((uint8_t *)helpCmd);
	uartSendString((uint8_t *)"\r\n");
	uartSendString((uint8_t *)ledOnCmd);
	uartSendString((uint8_t *)"\r\n");
	uartSendString((uint8_t *)ledOffCmd);
	uartSendString((uint8_t *)"\r\n");
	uartSendString((uint8_t *)ledToggleCmd);
	uartSendString((uint8_t *)"\r\n");
	uartSendString((uint8_t *)statusCmd);
	uartSendString((uint8_t *)"\r\n");
}


/**
 * @brief Determina si la linea recibida es ejecutable.
 *
 * Las lineas que comienzan con `#` o `//` se interpretan como entradas no
 * validas para ejecucion. En los demas casos se normaliza el texto a
 * mayusculas y se habilita su comparacion con el conjunto de comandos
 * soportados.
 *
 * @return true si la linea puede pasar a ejecucion.
 * @return false si la linea debe rechazarse.
 */
bool_t cmdProcessLine(void) {
	if (buffer[0] == '#') {
		return false;
	} else if (buffer[0] == '/' && buffer[1] == '/') {
		return false;
	}
	strToUpper(buffer);
	return true;
}

/**
 * @brief Convierte a mayusculas los caracteres alfabeticos de una cadena.
 *
 * @param str Puntero a la cadena a convertir.
 */
static void strToUpper(uint8_t *str)
{
	if (str == NULL) {
		return;
	}
	uint8_t i = 0;

	while (str[i] != '\0') {
		if(str[i] >= 'a' && str[i] <= 'z') {
			buffer[i] = buffer[i] - LOWER_TO_UPPER;
		}
		i++;
	}
}
