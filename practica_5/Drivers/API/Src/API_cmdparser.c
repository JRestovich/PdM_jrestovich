/*
 * API_cmdparser.c
 *
 *  Created on: Apr 8, 2026
 *      Author: joaquin
 */

#include "API_cmdparser.h"
#include "API_uart.h"

void cmdParserInit(void) {
	uartInit();
}

void cmdPoll(void) {
	uint8_t rx;
	uartReceiveStringSize(&rx, 1);

	if (uartReceiveOk()) {
		uartSendStringSize(&rx, 1);
	}
}

void cmdPrintHelp(void) {

}
