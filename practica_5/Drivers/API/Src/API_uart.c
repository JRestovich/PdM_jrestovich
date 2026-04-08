/*
 * API_uart.c
 *
 *  Created on: Apr 7, 2026
 *      Author: joaquin
 */

#include "API_uart.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "string.h"

#define TIMEOUT_UART2 10
UART_HandleTypeDef huart2;

bool_t uartInit()
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&huart2) != HAL_OK) {
		return false;
	} else {
		char buffer[200];
		sprintf(buffer,
				"UART Config:\r\n"
				"BaudRate: %lu\r\n"
				"WordLength: %d\r\n"
				"StopBits: %d\r\n"
				"Parity: %d\r\n",
				huart2.Init.BaudRate,
				huart2.Init.WordLength,
				huart2.Init.StopBits,
				huart2.Init.Parity);

		uartSendStringSize((uint8_t*)buffer, strlen(buffer));
	    return true;
	}
}

void uartSendString(uint8_t * pstring)
{
	if (pstring == NULL) {
		return;
	}
}

void uartSendStringSize(uint8_t * pstring, uint16_t size)
{
	if (pstring == NULL) {
		return;
	}
}

void uartReceiveStringSize(uint8_t * pstring, uint16_t size)
{
	if (pstring == NULL) {
		return;
	}
}
