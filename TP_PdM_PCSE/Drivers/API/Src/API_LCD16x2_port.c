/*
 * API_LCD16x2_port.c
 *
 *  Created on: Apr 17, 2026
 *      Author: joaquin
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

	return API_I2C_DEVICE_Rx(port, port->deviceAddress, value);
}

bool_t API_LCD16x2_port_Is_Alive(I2C_Device_t *port) {
	uint8_t value;

	if (port == NULL) {
		return false;
	}

	return API_LCD16x2_port_Read_Byte(port, &value);
}
