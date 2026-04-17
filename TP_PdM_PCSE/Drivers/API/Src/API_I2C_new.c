/*
 * API_I2C_new.c
 *
 *  Created on: Apr 17, 2026
 *      Author: joaquin
 */

#include "API_I2C_new.h"

#define DEFAULT_CLK_SPEED 100000
#define DEFAULT_SDA_I2C1 GPIO_PIN_9 // B
#define DEFAULT_SCL_I2C1 GPIO_PIN_8 // B

#define DEFAULT_SDA_I2C2 GPIO_PIN_3 // B
#define DEFAULT_SCL_I2C2 GPIO_PIN_10 // B

#define DEFAULT_SDA_I2C3 GPIO_PIN_4 // B
#define DEFAULT_SCL_I2C3 GPIO_PIN_8 // A

static bool_t initDefaultHi2c(I2C_Device_t* device, I2C_TypeDef *i2cInstance);
static bool_t initDefaultHw(I2C_TypeDef *i2cInstance);
static bool_t i2cDeviceReady(I2C_Device_t* device, uint16_t address);

bool_t API_I2C_DefaultConfig(I2C_Device_t* device, I2C_TypeDef *i2cInstance) {
	if (device == NULL) {
		return false;
	}
	if (!initDefaultHi2c(device, i2cInstance) || !initDefaultHw(i2cInstance)) {
		return false;
	}

	return true;
}

// TODO agregar enum de errores
bool_t API_I2C_Init(I2C_Device_t* device, uint16_t address) {
	if (device == NULL) {
		return false;
	}
	if (HAL_I2C_Init(&device->hi2c) != HAL_OK) {
		return false;
	}

	if (!i2cDeviceReady(device, address)) {
		return false;
	}

	device->deviceAddress = address;

	return true;
}

bool_t API_I2C_Tx(I2C_Device_t* device, uint16_t address, uint8_t *value, uint16_t size) {
	if (device == NULL || value == NULL || size == 0U) {
		return false;
	}
	return HAL_I2C_Master_Transmit(&device->hi2c, address << 1, value, size, HAL_MAX_DELAY) == HAL_OK;
}

bool_t API_I2C_Rx(I2C_Device_t* device, uint16_t address, uint8_t* value) {
	if (device == NULL || value == NULL) {
		return false;
	}
	return HAL_I2C_Master_Receive(&device->hi2c, address << 1, value, 1, HAL_MAX_DELAY) == HAL_OK;
}

void API_I2C_SetAddress(I2C_Device_t * device, uint16_t address) {
	device->deviceAddress = address;
}

/***********************************/
static bool_t initDefaultHi2c(I2C_Device_t* device, I2C_TypeDef *i2cInstance) {
	if (device == NULL) {
		return false;
	}

	device->hi2c.Instance = i2cInstance;
	device->hi2c.Init.ClockSpeed = DEFAULT_CLK_SPEED;
	device->hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
	device->hi2c.Init.OwnAddress1 = 0;
	device->hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	device->hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	device->hi2c.Init.OwnAddress2 = 0;
	device->hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	device->hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	return true;
}

static bool_t initDefaultHw(I2C_TypeDef *i2cInstance) {
	GPIO_InitTypeDef GPIO_InitStruct_A = {0};
	GPIO_InitTypeDef GPIO_InitStruct_B = {0};

	GPIO_InitStruct_A.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct_A.Pull = GPIO_NOPULL;
	GPIO_InitStruct_A.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct_A.Alternate = GPIO_AF4_I2C1;

	GPIO_InitStruct_B = GPIO_InitStruct_A;

	if (i2cInstance == I2C1) {
		GPIO_InitStruct_B.Pin = DEFAULT_SDA_I2C1 | DEFAULT_SCL_I2C1;
		__HAL_RCC_GPIOB_CLK_ENABLE();
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_B);
		__HAL_RCC_I2C1_CLK_ENABLE();

	} else if (i2cInstance == I2C2) {
		GPIO_InitStruct_B.Pin = DEFAULT_SDA_I2C2 | DEFAULT_SCL_I2C2;
		__HAL_RCC_GPIOB_CLK_ENABLE();
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_B);
		__HAL_RCC_I2C2_CLK_ENABLE();
	} else if (i2cInstance == I2C3) {
		GPIO_InitStruct_A.Pin = DEFAULT_SCL_I2C3;
		GPIO_InitStruct_B.Pin = DEFAULT_SDA_I2C3;
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct_A);
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct_B);
		__HAL_RCC_I2C3_CLK_ENABLE();
	} else {
		return false;
	}

	return true;
}

static bool_t i2cDeviceReady(I2C_Device_t* device, uint16_t address) {
	return HAL_I2C_IsDeviceReady(&device->hi2c, address << 1, 3, HAL_MAX_DELAY) == HAL_OK;
}
