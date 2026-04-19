/**
 * @file API_LCD16x2_port.c
 * @brief Implementación del port I2C para el display LCD 16x2.
 *
 * Este archivo contiene la lógica de acceso físico al expansor I2C utilizado
 * por el módulo LCD 16x2. La documentación de la API pública se mantiene en
 * `API_LCD16x2_port.h`, por lo que aquí no se vuelve a documentar dicha
 * interfaz.
 */

#include "API_LCD16x2_port.h"

bool_t API_LCD16x2_port_Init(I2C_Device_t *port, uint16_t address, I2C_TypeDef *i2cInstance) {
	if (port == NULL) {
		return false;
	}
	port->deviceAddress = address;
	if (!API_I2C_DEVICE_DefaultConfig(port, i2cInstance)) {
		return false;
	}
	if (!API_I2C_DEVICE_Init(port, port->deviceAddress)) {
		return false;
	}
	return true;
}

bool_t API_LCD16x2_port_Write_Byte(I2C_Device_t *port, uint8_t valor) {
	if (port == NULL) {
		return false;
	}
	return API_I2C_DEVICE_Tx(port, port->deviceAddress, &valor, sizeof(valor));
}

bool_t API_LCD16x2_port_Read_Byte(I2C_Device_t *port, uint8_t *value) {
	if (port == NULL || value == NULL) {
		return false;
	}

	return API_I2C_DEVICE_Rx(port, port->deviceAddress, value, 1);
}

bool_t API_LCD16x2_port_Is_Alive(I2C_Device_t *port) {
	uint8_t value;

	if (port == NULL) {
		return false;
	}

	return API_LCD16x2_port_Read_Byte(port, &value);
}
