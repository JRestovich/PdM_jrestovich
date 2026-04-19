/**
 * @file API_MPR121_port.c
 * @brief Low-level MPR121 driver implementation.
 *
 * Created on: Apr 13, 2026
 * Author: joaquin
 */

#include "API_MPR121_port.h"
#include "API_I2C_DEVICE.h"

#define MPR121_ADDR 0x5A ///< I2C address of the MPR121 device.
#define MPR121_REG_SOFTRESET 0x80 ///< Soft reset register address.
#define MPR121_REG_ECR 0x5E ///< Electrode configuration register address.
#define MPR121_REG_TOUCH_THRESHOLD_BASE 0x41 ///< Base address of touch threshold registers.
#define MPR121_REG_DEBOUNCE 0x5B ///< Debounce register address.
#define MPR121_REG_CONFIG1 0x5C ///< First filter configuration register address.
#define MPR121_REG_CONFIG2 0x5D ///< Second filter configuration register address.

#define MPR121_SOFTRESET_CMD 0x63 ///< Command value used for device reset.
#define MPR121_ECR_STOP_MODE 0x00 ///< ECR value used to stop all electrodes.
#define MPR121_TOUCH_THRESHOLD 12 ///< Touch threshold applied to each electrode.
#define MPR121_RELEASE_THRESHOLD 6 ///< Release threshold applied to each electrode.
#define MPR121_DEBOUNCE_VALUE 0x00 ///< Debounce configuration value.
#define MPR121_CONFIG1_VALUE 0x10 ///< CONFIG1 register value.
#define MPR121_CONFIG2_VALUE 0x20 ///< CONFIG2 register value.
#define MPR121_ECR_RUN_12_ELECTRODES 0x8C ///< ECR value used to enable 12 electrodes.

#define BUFFER_SIZE 25 ///< Number of bytes sent during threshold configuration.
#define TWO_BYTES 2 ///< Length of two-byte transfers.
#define KEYS_QTY 12 ///< Number of keypad electrodes used.

//static I2C_HandleTypeDef hi2c;
static bool_t initOk = false; ///< Indicates whether the port layer is initialized.
static I2C_Device_t device = {0}; ///< I2C device descriptor used by the driver.

/**
 * @brief Configures the MPR121 after the I2C interface is initialized.
 *
 * @return true Configuration completed successfully.
 * @return false Configuration failed.
 */
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
	uint8_t data[TWO_BYTES] = {0};

	if (keys == NULL || !initOk) {
		return false;
	}

	// Leer Touch Status LSB/MSB en una sola operación de lectura de registro.
	if (!API_I2C_DEVICE_MemRead(&device, 0x00, I2C_MEMADD_SIZE_8BIT, data, TWO_BYTES)) {
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

/*****************************************************/
static bool_t initModule() {
	uint8_t pData[BUFFER_SIZE];

	    // =========================================================
	    // 0. Soft reset
	    // Lleva al MPR121 a un estado conocido antes de configurar
	    // =========================================================
	    pData[0] = MPR121_REG_SOFTRESET;
	    pData[1] = MPR121_SOFTRESET_CMD;
	    if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, TWO_BYTES)) {
			return false;
		}
	    HAL_Delay(10);

	    // =========================================================
	    // 1. Poner el MPR121 en STOP mode (deshabilita electrodos)
	    // Esto es obligatorio antes de configurar registros
	    // =========================================================
	    pData[0] = MPR121_REG_ECR;
	    pData[1] = MPR121_ECR_STOP_MODE;
	    if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, TWO_BYTES)) {
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
	    for (uint8_t i = 0; i < KEYS_QTY; i++)
	    {
	    	pData[1 + i*2] = MPR121_TOUCH_THRESHOLD;
	    	pData[2 + i*2] = MPR121_RELEASE_THRESHOLD;
	    }
	    if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, BUFFER_SIZE)) {
			return false;
		}

	    // =========================================================
	    // 3. Configuración de filtros y adquisición (AFE)
	    // =========================================================
	    pData[0] = MPR121_REG_CONFIG1;
	    pData[1] = MPR121_CONFIG1_VALUE;
	    if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, TWO_BYTES)) {
			return false;
		}

	    pData[0] = MPR121_REG_CONFIG2;
		pData[1] = MPR121_CONFIG2_VALUE;
		if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, TWO_BYTES)) {
			return false;
		}

	    // =========================================================
	    // 4. Configurar debounce
	    // Se deja en 0 como en la versión Arduino que sí detecta
	    // =========================================================
	    pData[0] = MPR121_REG_DEBOUNCE;
		pData[1] = MPR121_DEBOUNCE_VALUE;
		// pData[1] = 0x22;
		if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, TWO_BYTES)) {
			return false;
		}

	    // =========================================================
	    // 5. Habilitar electrodos
	    // Se usa 0x8C como en Arduino: 12 electrodos + baseline tracking
	    // =========================================================
		pData[0] = MPR121_REG_ECR;
		pData[1] = MPR121_ECR_RUN_12_ELECTRODES;
		if (!API_I2C_DEVICE_Tx(&device, device.deviceAddress, pData, TWO_BYTES)) {
			return false;
		}

	    return true;
}
