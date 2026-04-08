/*
 * API_cmdparser.h
 *
 *  Created on: Apr 8, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_CMDPARSER_H_
#define API_INC_API_CMDPARSER_H_

typedef enum {
	CMD_OK=0,
	CMD_ERR_OVERFLOW,
	CMD_ERR_SYNTAX,
	CMD_ERR_UNKNOWN,
	CMD_ERR_ARG
} cmd_status_t;

typedef enum {
	CMD_IDLE=0,
	CMD_RECEIVING,
	CMD_PROCESS,
	CMD_EXEC,
	CMD_ERROR
} cmdParser_state_t;


void cmdParserInit(void);
void cmdPoll(void);
void cmdPrintHelp(void);

#endif /* API_INC_API_CMDPARSER_H_ */
