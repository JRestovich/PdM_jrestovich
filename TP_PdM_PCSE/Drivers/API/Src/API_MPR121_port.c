/*
 * API_MPR121_port.c
 *
 *  Created on: Apr 13, 2026
 *      Author: joaquin
 */

#include "API_MPR121_port.h"
#include "API_I2C_DEVICE.h"

#define MPR121_ADDR 0x5A
#define MPR121_REG_SOFTRESET 0x80
#define MPR121_REG_ECR 0x5E
#define MPR121_REG_TOUCH_THRESHOLD_BASE 0x41
#define MPR121_REG_DEBOUNCE 0x5B
#define MPR121_REG_CONFIG1 0x5C
#define MPR121_REG_CONFIG2 0x5D

#define MPR121_SOFTRESET_CMD 0x63
#define MPR121_ECR_STOP_MODE 0x00
#define MPR121_TOUCH_THRESHOLD 12
#define MPR121_RELEASE_THRESHOLD 6
#define MPR121_DEBOUNCE_VALUE 0x00
#define MPR121_CONFIG1_VALUE 0x10
#define MPR121_CONFIG2_VALUE 0x20
#define MPR121_ECR_RUN_12_ELECTRODES 0x8C

//static I2C_HandleTypeDef hi2c;
static bool_t initOk = false;
static I2C_Device_t device = {0};

static bool_t initModule();

bool_t API_MPR121_port_init() {

	if (!API_I2C_DEVICE_DefaultConfig(&device, I2C1)) {
		return false;
	}
	if (!API_I2C_DEVICE_Init(&device, MPR121_ADDR)) {
		return false;
	}
	if (!initModule()) {
		return false;
	}

	initOk = true;
	return true;
}

bool_t API_MPR121_port_readKeys(uint16_t *keys) {
	uint8_t data[2] = {0};

	if (keys == NULL || !initOk) {
		return false;
	}

	// Leer Touch Status LSB/MSB en una sola operación de lectura de registro.
	if (!API_I2C_DEVICE_MemRead(&device, 0x00, I2C_MEMADD_SIZE_8BIT, data, 2)) {
		return false;
	}

	// =========================================================
	// Combinar bytes:
	// data[0] → ELE0–ELE7
	// data[1] → ELE8–ELE11 (+ flags)
	// =========================================================
	*keys = ((uint16_t)data[1] << 8) | data[0];

	// Solo nos interesan 12 bits (ELE0–ELE11)
	*keys &= 0x0FFF;

	return true;
}

bool_t API_MPR121_port_getKeyValue(MPR121_keyValue key) {
//	HAL_I2C_Mem_Write
	return false;
}

/*****************************************************/
static bool_t initModule() {
	uint8_t pData[25];

	    // =========================================================
	    // 0. Soft reset
	    // Lleva al MPR121 a un estado conocido antes de configurar
	    // =========================================================
	    pData[0] = MPR121_REG_SOFTRESET;
	    pData[1] = MPR121_SOFTRESET_CMD;
	    if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, 2)) {
			return false;
		}
	    HAL_Delay(10);

	    // =========================================================
	    // 1. Poner el MPR121 en STOP mode (deshabilita electrodos)
	    // Esto es obligatorio antes de configurar registros
	    // =========================================================
	    pData[0] = MPR121_REG_ECR;
	    pData[1] = MPR121_ECR_STOP_MODE;
	    if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, 2)) {
			return false;
		}
	    HAL_Delay(5);

	    // =========================================================
	    // 2. Configurar thresholds (TOUCH / RELEASE)
	    // - Touch threshold: sensibilidad al toque
	    // - Release threshold: histéresis (evita rebotes)
	    //
	    // Valores típicos:
	    //   Touch   = 12
	    //   Release = 6
	    // =========================================================
	    pData[0] = MPR121_REG_TOUCH_THRESHOLD_BASE;
	    // Los registros siguientes son consecutivos, por eso el for avanza por offset.
	    for (uint8_t i = 0; i < 12; i++)
	    {
	    	pData[1 + i*2] = MPR121_TOUCH_THRESHOLD;
	    	pData[2 + i*2] = MPR121_RELEASE_THRESHOLD;
	    }
	    if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, 25)) {
			return false;
		}

	    // =========================================================
	    // 3. Configuración de filtros y adquisición (AFE)
	    // =========================================================
	    pData[0] = MPR121_REG_CONFIG1;
	    pData[1] = MPR121_CONFIG1_VALUE;
	    if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, 2)) {
			return false;
		}

	    pData[0] = MPR121_REG_CONFIG2;
		pData[1] = MPR121_CONFIG2_VALUE;
		if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, 2)) {
			return false;
		}

	    // =========================================================
	    // 4. Configurar debounce
	    // Se deja en 0 como en la versión Arduino que sí detecta
	    // =========================================================
	    pData[0] = MPR121_REG_DEBOUNCE;
		pData[1] = MPR121_DEBOUNCE_VALUE;
		// pData[1] = 0x22;
		if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, 2)) {
			return false;
		}

	    // =========================================================
	    // 5. Habilitar electrodos
	    // Se usa 0x8C como en Arduino: 12 electrodos + baseline tracking
	    // =========================================================
		pData[0] = MPR121_REG_ECR;
		pData[1] = MPR121_ECR_RUN_12_ELECTRODES;
		if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, 2)) {
			return false;
		}

	    return true;
}
