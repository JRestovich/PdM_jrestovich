/**
 * @file API_uart.h
 * @brief Interfaz de alto nivel para la comunicacion serie por UART.
 *
 * Este modulo encapsula la inicializacion del periferico UART2 y ofrece
 * primitivas simples para enviar y recibir cadenas de bytes.
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Alias de tipo booleano utilizado por la API.
 */
typedef bool bool_t;

/**
 * @brief Inicializa el periferico UART2 con una configuracion predefinida.
 *
 * La configuracion aplicada es de 115200 baudios, 8 bits de datos,
 * 1 bit de stop, sin paridad y con transmision/recepcion habilitadas.
 * Si la inicializacion es exitosa, el modulo envia por UART un mensaje
 * con la configuracion establecida.
 *
 * @return true si la inicializacion se realizo correctamente.
 * @return false si ocurrio un error durante la inicializacion.
 */
bool_t uartInit();

/**
 * @brief Envia una cadena terminada en `\0` por UART.
 *
 * La longitud se calcula internamente usando la terminacion nula de la
 * cadena recibida.
 *
 * @param pstring Puntero a la cadena a transmitir.
 */
void uartSendString(uint8_t * pstring);

/**
 * @brief Envia una cantidad fija de bytes por UART.
 *
 * La transmision solo se realiza si el puntero es valido y el tamano se
 * encuentra dentro del rango aceptado por el modulo.
 *
 * @param pstring Puntero al buffer de datos a transmitir.
 * @param size Cantidad de bytes a enviar.
 */
void uartSendStringSize(uint8_t * pstring, uint16_t size);

/**
 * @brief Recibe una cantidad fija de bytes desde UART.
 *
 * El resultado de la operacion puede consultarse posteriormente con
 * ::uartReceiveOk.
 *
 * @param pstring Puntero al buffer donde se almacenaran los datos recibidos.
 * @param size Cantidad de bytes a recibir.
 */
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);

/**
 * @brief Informa si la ultima recepcion UART finalizo correctamente.
 *
 * @return true si la ultima recepcion termino con estado `HAL_OK`.
 * @return false si la ultima recepcion fallo o no se completo correctamente.
 */
bool_t uartReceiveOk();

#endif /* API_INC_API_UART_H_ */
