/**
 * @file API_I2C_DEVICE.h
 * @brief Interfaz genérica para configurar y operar dispositivos I2C.
 *
 * Este módulo encapsula la configuración de un dispositivo I2C a partir de un
 * handler HAL y la dirección del esclavo asociado. La API permite inicializar
 * el periférico, realizar transferencias básicas y consultar o modificar los
 * principales parámetros de configuración del bus.
 */

#ifndef API_INC_API_I2C_DEVICE_H_
#define API_INC_API_I2C_DEVICE_H_

#include <stdio.h>
#include "typedefs.h"

/**
 * @brief Descriptor de un dispositivo conectado al bus I2C.
 *
 * Agrupa el handler HAL utilizado para acceder al periférico I2C y la dirección
 * del dispositivo esclavo con el que opera el módulo.
 */
typedef struct {
    I2C_HandleTypeDef hi2c;
    uint16_t deviceAddress;
} I2C_Device_t;

/**
 * @brief Carga en el descriptor una configuración I2C por defecto.
 *
 * Inicializa el handler HAL del dispositivo con parámetros estándar y prepara
 * el hardware asociado a la instancia I2C indicada.
 *
 * @param device Puntero al descriptor del dispositivo a configurar.
 * @param i2cInstance Instancia del periférico I2C a asociar.
 *
 * @return `true` si la configuración por defecto se aplicó correctamente,
 *         `false` si los parámetros son inválidos o falló la preparación del
 *         hardware.
 */
bool_t API_I2C_DEVICE_DefaultConfig(I2C_Device_t *device, I2C_TypeDef *i2cInstance);

/**
 * @brief Inicializa el periférico I2C y verifica la disponibilidad del
 *        dispositivo esclavo.
 *
 * Ejecuta la inicialización HAL del periférico previamente configurado y
 * comprueba que el esclavo indicado responda sobre el bus.
 *
 * @param device Puntero al descriptor del dispositivo a inicializar.
 * @param address Dirección I2C de 7 bits del dispositivo esclavo.
 *
 * @return `true` si la inicialización finalizó correctamente y el dispositivo
 *         respondió, `false` en caso contrario.
 */
bool_t API_I2C_DEVICE_Init(I2C_Device_t *device, uint16_t address);

/**
 * @brief Transmite un bloque de datos a una dirección I2C.
 *
 * Envía `size` bytes por el bus I2C hacia la dirección indicada utilizando el
 * periférico asociado al descriptor.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param address Dirección I2C de destino.
 * @param value Puntero al buffer con los datos a transmitir.
 * @param size Cantidad de bytes a enviar.
 *
 * @return `true` si la transmisión se completó correctamente, `false` si hubo
 *         un error o los parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_Tx(I2C_Device_t *device, uint16_t address, uint8_t *value, uint16_t size);

/**
 * @brief Recibe un byte desde una dirección I2C.
 *
 * Solicita la recepción de un byte a la dirección indicada utilizando el
 * periférico asociado al descriptor.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param address Dirección I2C del esclavo origen.
 * @param value Puntero donde se almacenará el byte recibido.
 *
 * @return `true` si la recepción se completó correctamente, `false` si hubo un
 *         error o los parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_Rx(I2C_Device_t *device, uint16_t address, uint8_t *value);

/**
 * @brief Lee uno o más bytes desde un registro interno del dispositivo I2C.
 *
 * Ejecuta una operación de lectura de memoria partiendo de la dirección de
 * registro indicada y almacena los datos recibidos en el buffer provisto.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param registerAddress Dirección del registro interno a leer.
 * @param registerAddressSize Tamaño, en bytes, de la dirección de registro.
 * @param values Puntero al buffer donde se almacenarán los datos leídos.
 * @param size Cantidad de bytes a leer.
 *
 * @return `true` si la lectura se completó correctamente, `false` si ocurrió
 *         un error o los parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_MemRead(I2C_Device_t *device,
		uint16_t registerAddress,
		uint16_t registerAddressSize,
		uint8_t *values,
		uint16_t size);

/**
 * @brief Actualiza la dirección I2C almacenada en el descriptor.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param address Nueva dirección I2C a almacenar.
 *
 * @return `true` si la dirección se actualizó correctamente, `false` si el
 *         parámetro `device` es inválido.
 */
bool_t API_I2C_DEVICE_SetAddress(I2C_Device_t *device, uint16_t address);

/**
 * @brief Obtiene la dirección I2C almacenada en el descriptor.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param address Puntero donde se almacenará la dirección.
 *
 * @return `true` si la lectura se realizó correctamente, `false` si los
 *         parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_GetAddress(const I2C_Device_t *device, uint16_t *address);

/**
 * @brief Asocia una nueva instancia de periférico I2C al descriptor.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param i2cInstance Nueva instancia I2C a asociar.
 *
 * @return `true` si la instancia se actualizó correctamente, `false` si los
 *         parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_SetInstance(I2C_Device_t *device, I2C_TypeDef *i2cInstance);

/**
 * @brief Obtiene la instancia de periférico I2C asociada al descriptor.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param i2cInstance Puntero donde se almacenará la instancia I2C.
 *
 * @return `true` si la lectura se realizó correctamente, `false` si los
 *         parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_GetInstance(const I2C_Device_t *device, I2C_TypeDef **i2cInstance);

/**
 * @brief Actualiza la velocidad de reloj configurada para el periférico I2C.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param clockSpeed Nueva velocidad de reloj, en hertz.
 *
 * @return `true` si el valor se actualizó correctamente, `false` si el
 *         parámetro `device` es inválido.
 */
bool_t API_I2C_DEVICE_SetClockSpeed(I2C_Device_t *device, uint32_t clockSpeed);

/**
 * @brief Obtiene la velocidad de reloj configurada para el periférico I2C.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param clockSpeed Puntero donde se almacenará la velocidad configurada.
 *
 * @return `true` si la lectura se realizó correctamente, `false` si los
 *         parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_GetClockSpeed(const I2C_Device_t *device, uint32_t *clockSpeed);

/**
 * @brief Actualiza el ciclo de trabajo configurado para el periférico I2C.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param dutyCycle Nuevo ciclo de trabajo a configurar.
 *
 * @return `true` si el valor se actualizó correctamente, `false` si el
 *         parámetro `device` es inválido.
 */
bool_t API_I2C_DEVICE_SetDutyCycle(I2C_Device_t *device, uint32_t dutyCycle);

/**
 * @brief Obtiene el ciclo de trabajo configurado para el periférico I2C.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param dutyCycle Puntero donde se almacenará el ciclo de trabajo.
 *
 * @return `true` si la lectura se realizó correctamente, `false` si los
 *         parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_GetDutyCycle(const I2C_Device_t *device, uint32_t *dutyCycle);

/**
 * @brief Actualiza la primera dirección propia configurada en el periférico.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param ownAddress1 Nueva dirección propia primaria.
 *
 * @return `true` si el valor se actualizó correctamente, `false` si el
 *         parámetro `device` es inválido.
 */
bool_t API_I2C_DEVICE_SetOwnAddress1(I2C_Device_t *device, uint32_t ownAddress1);

/**
 * @brief Obtiene la primera dirección propia configurada en el periférico.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param ownAddress1 Puntero donde se almacenará la dirección propia primaria.
 *
 * @return `true` si la lectura se realizó correctamente, `false` si los
 *         parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_GetOwnAddress1(const I2C_Device_t *device, uint32_t *ownAddress1);

/**
 * @brief Actualiza el modo de direccionamiento del periférico I2C.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param addressingMode Nuevo modo de direccionamiento.
 *
 * @return `true` si el valor se actualizó correctamente, `false` si el
 *         parámetro `device` es inválido.
 */
bool_t API_I2C_DEVICE_SetAddressingMode(I2C_Device_t *device, uint32_t addressingMode);

/**
 * @brief Obtiene el modo de direccionamiento configurado en el periférico I2C.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param addressingMode Puntero donde se almacenará el modo configurado.
 *
 * @return `true` si la lectura se realizó correctamente, `false` si los
 *         parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_GetAddressingMode(const I2C_Device_t *device, uint32_t *addressingMode);

/**
 * @brief Actualiza el modo de doble direccionamiento del periférico I2C.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param dualAddressMode Nuevo modo de doble direccionamiento.
 *
 * @return `true` si el valor se actualizó correctamente, `false` si el
 *         parámetro `device` es inválido.
 */
bool_t API_I2C_DEVICE_SetDualAddressMode(I2C_Device_t *device, uint32_t dualAddressMode);

/**
 * @brief Obtiene el modo de doble direccionamiento configurado.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param dualAddressMode Puntero donde se almacenará el modo configurado.
 *
 * @return `true` si la lectura se realizó correctamente, `false` si los
 *         parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_GetDualAddressMode(const I2C_Device_t *device, uint32_t *dualAddressMode);

/**
 * @brief Actualiza la segunda dirección propia configurada en el periférico.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param ownAddress2 Nueva dirección propia secundaria.
 *
 * @return `true` si el valor se actualizó correctamente, `false` si el
 *         parámetro `device` es inválido.
 */
bool_t API_I2C_DEVICE_SetOwnAddress2(I2C_Device_t *device, uint32_t ownAddress2);

/**
 * @brief Obtiene la segunda dirección propia configurada en el periférico.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param ownAddress2 Puntero donde se almacenará la dirección propia secundaria.
 *
 * @return `true` si la lectura se realizó correctamente, `false` si los
 *         parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_GetOwnAddress2(const I2C_Device_t *device, uint32_t *ownAddress2);

/**
 * @brief Actualiza el modo de general call configurado en el periférico I2C.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param generalCallMode Nuevo modo de general call.
 *
 * @return `true` si el valor se actualizó correctamente, `false` si el
 *         parámetro `device` es inválido.
 */
bool_t API_I2C_DEVICE_SetGeneralCallMode(I2C_Device_t *device, uint32_t generalCallMode);

/**
 * @brief Obtiene el modo de general call configurado en el periférico I2C.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param generalCallMode Puntero donde se almacenará el modo configurado.
 *
 * @return `true` si la lectura se realizó correctamente, `false` si los
 *         parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_GetGeneralCallMode(const I2C_Device_t *device, uint32_t *generalCallMode);

/**
 * @brief Actualiza el modo no-stretch configurado en el periférico I2C.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param noStretchMode Nuevo modo no-stretch.
 *
 * @return `true` si el valor se actualizó correctamente, `false` si el
 *         parámetro `device` es inválido.
 */
bool_t API_I2C_DEVICE_SetNoStretchMode(I2C_Device_t *device, uint32_t noStretchMode);

/**
 * @brief Obtiene el modo no-stretch configurado en el periférico I2C.
 *
 * @param device Puntero al descriptor del dispositivo I2C.
 * @param noStretchMode Puntero donde se almacenará el modo configurado.
 *
 * @return `true` si la lectura se realizó correctamente, `false` si los
 *         parámetros son inválidos.
 */
bool_t API_I2C_DEVICE_GetNoStretchMode(const I2C_Device_t *device, uint32_t *noStretchMode);

#endif /* API_INC_API_I2C_DEVICE_H_ */
