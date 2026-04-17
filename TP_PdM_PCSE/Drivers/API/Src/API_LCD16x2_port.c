/*
 * API_LCD16x2_port.c
 *
 *  Created on: Apr 17, 2026
 *      Author: joaquin
 */
#include "API_LCD16x2_port.h"


bool_t API_LCD16x2_port_Init(LCD16x2_port* port, uint16_t address, I2C_TypeDef* i2cInstance) {
	if (port == NULL) {
		return false;
	}
	port->address = address;
	if (!API_I2C_DefaultConfig(&port->hi2c, i2cInstance)) {
		return false;
	}
	if (!API_I2C_Init(&port->hi2c, port->address)) {
		return false;
	}
	return true;
}

bool_t API_LCD16x2_port_Write_Byte(LCD16x2_port* port, uint8_t valor) {
	if (port == NULL) {
		return false;
	}
	return API_I2C_Tx(&port->hi2c, port->address, &valor, sizeof(valor));
}

bool_t API_LCD16x2_port_Read_Byte(LCD16x2_port *port, uint8_t *value) {
	if (port == NULL || value == NULL) {
		return false;
	}

	return API_I2C_Rx(&port->hi2c, port->address, value);
}

bool_t API_LCD16x2_port_Is_Alive(LCD16x2_port *port) {
	uint8_t value;

	if (port == NULL) {
		return false;
	}

	return API_LCD16x2_port_Read_Byte(port, &value);
}
