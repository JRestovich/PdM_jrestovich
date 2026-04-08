/**
 * @file API_cmdparser.h
 * @brief Interfaz del modulo de interpretacion de comandos por UART.
 *
 * Este modulo implementa un parser sencillo basado en una maquina de
 * estados. Permite recibir comandos de texto por UART, validarlos y
 * ejecutar acciones asociadas, como consultar ayuda o controlar un LED.
 */

#ifndef API_INC_API_CMDPARSER_H_
#define API_INC_API_CMDPARSER_H_

/**
 * @brief Estados de resultado asociados al procesamiento de comandos.
 */
typedef enum {
	/** Comando procesado correctamente. */
	CMD_OK=0,
	/** El buffer de recepcion excedio su capacidad. */
	CMD_ERR_OVERFLOW,
	/** Se detecto un error de sintaxis. */
	CMD_ERR_SYNTAX,
	/** El comando recibido no esta contemplado por el parser. */
	CMD_ERR_UNKNOWN,
	/** Los argumentos o el formato del comando son invalidos. */
	CMD_ERR_ARG
} cmd_status_t;

/**
 * @brief Estados internos de la maquina de estados del parser.
 */
typedef enum {
	/** Espera de nuevos caracteres desde UART. */
	CMD_IDLE=0,
	/** Recepcion progresiva de un comando. */
	CMD_RECEIVING,
	/** Etapa de validacion y normalizacion de la linea recibida. */
	CMD_PROCESS,
	/** Ejecucion del comando reconocido. */
	CMD_EXEC,
	/** Estado de error del parser. */
	CMD_ERROR
} cmdParser_state_t;


/**
 * @brief Inicializa el parser de comandos y la UART asociada.
 *
 * Si la inicializacion de UART es exitosa, el parser queda listo en estado
 * de espera. En caso contrario, pasa a estado de error.
 */
void cmdParserInit(void);

/**
 * @brief Atiende la recepcion, validacion y ejecucion de comandos.
 *
 * Esta funcion debe invocarse periodicamente desde el lazo principal para
 * avanzar la maquina de estados del parser.
 */
void cmdPoll(void);

/**
 * @brief Envia por UART la lista de comandos disponibles.
 */
void cmdPrintHelp(void);

#endif /* API_INC_API_CMDPARSER_H_ */
