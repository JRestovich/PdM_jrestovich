/*
 * API_uart.h
 *
 *  Created on: Apr 7, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdint.h>
#include <stdbool.h>

typedef bool bool_t;

bool_t uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);
bool_t uartReceiveOk();

#endif /* API_INC_API_UART_H_ */
