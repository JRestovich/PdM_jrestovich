/*
 * API_MPR121_port.c
 *
 *  Created on: Apr 13, 2026
 *      Author: joaquin
 */

#include "API_MPR121_port.h"

#define MPR121_ADDR (0x5A << 1)  // HAL usa dirección desplazada
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

static I2C_HandleTypeDef hi2c;
static bool_t initOk = false;

static void initI2cPinsHw();
static bool_t initI2cHw();
static bool_t initModule();

bool_t API_MPR121_port_init() {
	initI2cPinsHw();
	if (!initI2cHw()) {
		return false;
	}
	if (!initModule()) {
		return false;
	}

	initOk = true;
	return true;
}

bool_t API_MPR121_port_readKeys(uint16_t *keys) {
	uint8_t reg = 0x00;       // registro inicial (Touch Status LSB)
	uint8_t data[2] = {0};

	if (keys == NULL) {
		return false;
	}

	// =========================================================
	// Leer 2 bytes desde 0x00 (auto-increment → 0x00 y 0x01)
	// =========================================================
	if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, &reg, 1, 1000) != HAL_OK) {
		return false;
	}

	if (HAL_I2C_Master_Receive(&hi2c, MPR121_ADDR, data, 2, 1000) != HAL_OK) {
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

static bool_t initI2cHw() {
    __HAL_RCC_I2C1_CLK_ENABLE();

    ///< 1. Configuración básica del periférico I2C

    hi2c.Instance = I2C1;   // Usamos I2C1 (ajustar según hardware)

    hi2c.Init.ClockSpeed      = 100000;             // 100 kHz (modo estándar)
    hi2c.Init.DutyCycle       = I2C_DUTYCYCLE_2;    // duty estándar
    hi2c.Init.OwnAddress1     = 0x00;               // no usamos dirección propia
    hi2c.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.OwnAddress2     = 0x00;
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

    ///< 2. Inicializar el periférico
    if (HAL_I2C_Init(&hi2c) != HAL_OK)
    {
        return false;
    }

    ///< 3. Verificación opcional: chequear que el MPR121 responde

    if (HAL_I2C_IsDeviceReady(&hi2c, MPR121_ADDR, 3, 100) != HAL_OK)
    {
        // El dispositivo no respondió
        return false;
    }

    ///< I2C listo para usar con el MPR121
    return true;
}

static void initI2cPinsHw() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* I2C1 en Nucleo-F446RE: PB8 = SCL, PB9 = SDA */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static bool_t initModule() {
	uint8_t pData[25];

	    // =========================================================
	    // 0. Soft reset
	    // Lleva al MPR121 a un estado conocido antes de configurar
	    // =========================================================
	    pData[0] = MPR121_REG_SOFTRESET;
	    pData[1] = MPR121_SOFTRESET_CMD;
	    if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
	    	return false;
	    }
	    HAL_Delay(10);

	    // =========================================================
	    // 1. Poner el MPR121 en STOP mode (deshabilita electrodos)
	    // Esto es obligatorio antes de configurar registros
	    // =========================================================
	    pData[0] = MPR121_REG_ECR;
	    pData[1] = MPR121_ECR_STOP_MODE;
	    if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
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
	    if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 25, 1000) != HAL_OK) {
			return false;
		}

	    // =========================================================
	    // 3. Configuración de filtros y adquisición (AFE)
	    // =========================================================
	    pData[0] = MPR121_REG_CONFIG1;
	    pData[1] = MPR121_CONFIG1_VALUE;
	    if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
			return false;
		}

	    pData[0] = MPR121_REG_CONFIG2;
		pData[1] = MPR121_CONFIG2_VALUE;
		if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
			return false;
		}

	    // =========================================================
	    // 4. Configurar debounce
	    // Se deja en 0 como en la versión Arduino que sí detecta
	    // =========================================================
	    pData[0] = MPR121_REG_DEBOUNCE;
		pData[1] = MPR121_DEBOUNCE_VALUE;
		// pData[1] = 0x22;
		if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
			return false;
		}

	    // =========================================================
	    // 5. Habilitar electrodos
	    // Se usa 0x8C como en Arduino: 12 electrodos + baseline tracking
	    // =========================================================
		pData[0] = MPR121_REG_ECR;
		pData[1] = MPR121_ECR_RUN_12_ELECTRODES;
		if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
			return false;
		}

	    return true;
}
