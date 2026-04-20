/**
 * @file API_LCD16x2_port.h
 * @brief Interfaz de port para el acceso físico al LCD 16x2 sobre I2C.
 *
 * Este módulo encapsula la comunicación de bajo nivel con el expansor I2C
 * utilizado por el display LCD 16x2. Provee una abstracción mínima para
 * inicializar el enlace, escribir bytes hacia el periférico y verificar la
 * presencia del dispositivo sobre el bus.
 */

#ifndef API_INC_API_LCD16X2_PORT_H_
#define API_INC_API_LCD16X2_PORT_H_

/********************************************************/
/* Includes */
#include <API_I2C_DEVICE.h>

/********************************************************/
/* Declaracion de Funciones Publicas */
/**
 * @brief Inicializa el port del LCD 16x2 sobre una instancia I2C dada.
 *
 * Configura un descriptor de dispositivo I2C con valores por defecto, lo
 * inicializa en hardware y registra la dirección del expansor asociado al LCD.
 *
 * @param port Puntero al descriptor I2C que operará como port del display.
 * @param address Dirección I2C del expansor asociado al LCD.
 * @param i2cInstance Instancia del periférico I2C a utilizar.
 *
 * @return `true` si la inicialización se completó correctamente, `false` en
 *         caso contrario.
 */
bool_t API_LCD16x2_port_Init(I2C_Device_t *port, uint16_t address, I2C_TypeDef *i2cInstance);

/**
 * @brief Escribe un byte en el expansor I2C del LCD.
 *
 * Transmite un único byte al dispositivo asociado al port.
 *
 * @param port Puntero al descriptor I2C del LCD.
 * @param valor Byte a transmitir.
 *
 * @return `true` si la escritura se realizó correctamente, `false` si ocurrió
 *         un error o los parámetros son inválidos.
 */
bool_t API_LCD16x2_port_Write_Byte(I2C_Device_t *port, uint8_t valor);

/**
 * @brief Lee un byte desde el expansor I2C del LCD.
 *
 * Solicita la recepción de un único byte desde el dispositivo asociado al port.
 *
 * @param port Puntero al descriptor I2C del LCD.
 * @param value Puntero donde se almacenará el byte recibido.
 *
 * @return `true` si la lectura se realizó correctamente, `false` si ocurrió un
 *         error o los parámetros son inválidos.
 */
bool_t API_LCD16x2_port_Read_Byte(I2C_Device_t *port, uint8_t *value);

/**
 * @brief Verifica si el dispositivo I2C asociado al LCD responde en el bus.
 *
 * Realiza una lectura simple para comprobar la disponibilidad del expansor
 * conectado al display.
 *
 * @param port Puntero al descriptor I2C del LCD.
 *
 * @return `true` si el dispositivo respondió correctamente, `false` en caso
 *         contrario.
 */
bool_t API_LCD16x2_port_Is_Alive(I2C_Device_t *port);

#endif /* API_INC_API_LCD16X2_PORT_H_ */
