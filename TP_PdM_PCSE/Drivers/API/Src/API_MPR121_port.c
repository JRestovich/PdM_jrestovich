/*
 * API_MPR121_port.c
 *
 *  Created on: Apr 13, 2026
 *      Author: joaquin
 */

#include "API_MPR121_port.h"

#define MPR121_ADDR (0x5A << 1)  // HAL usa dirección desplazada

static I2C_HandleTypeDef hi2c;
static bool_t initOk = false;

static bool_t initHw();
static bool_t initModule();

bool_t API_MPR121_port_init() {
	if (!initHw()) {
		return false;
	}
	if (!initModule()) {
		return false;
	}

	initOk = true;
	return true;
}

static bool_t initHw() {
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

static bool_t initModule() {
	uint8_t pData[25];

	    // =========================================================
	    // 1. Poner el MPR121 en STOP mode (deshabilita electrodos)
	    // Esto es obligatorio antes de configurar registros
	    // =========================================================
	    pData[0] = 0x5E;
	    pData[1] = 0x00;
	    if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
	    	return false;
	    }

	    // =========================================================
	    // 2. Configurar thresholds (TOUCH / RELEASE)
	    // - Touch threshold: sensibilidad al toque
	    // - Release threshold: histéresis (evita rebotes)
	    //
	    // Valores típicos:
	    //   Touch   = 12
	    //   Release = 6
	    // =========================================================
	    pData[0] = 0x41;
	    for (uint8_t i = 0; i < 12; i++)
	    {
	    	pData[1 + i*2] = 12;
	    	pData[2 + i*2] = 6;
	    }
	    if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 25, 1000) != HAL_OK) {
			return false;
		}

	    // =========================================================
	    // 3. Configuración de filtros y adquisición (AFE)
	    //
	    // 0x5C = AFE Configuration
	    //  - CDC: corriente de carga
	    //  - FFI: cantidad de samples del primer filtro
	    //
	    // 0x5D = Filter Configuration
	    //  - CDT: tiempo de carga
	    //  - SFI: segundo filtro
	    //  - ESI: intervalo de muestreo
	    //
	    // Estos valores son "default recomendados"
	    // =========================================================
	    pData[0] = 0x5C;
	    pData[1] = 0x10;
	    if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
			return false;
		}

	    pData[0] = 0x5D;
		pData[1] = 0x24;
		if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
			return false;
		}

	    // =========================================================
	    // 4. Configurar debounce
	    // Evita falsos triggers por ruido
	    // 0x22 = 2 lecturas consecutivas para validar cambio
	    // =========================================================
	    pData[0] = 0x5B;
		pData[1] = 0x22;
		if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
			return false;
		}

	    // =========================================================
	    // 5. (Opcional pero recomendado) Auto-config
	    // Ajusta automáticamente corriente y timing
	    // =========================================================
		pData[0] = 0x7B;
		pData[1] = 0x0B;
		if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
			return false;
		}

	    // =========================================================
	    // 6. Habilitar electrodos
	    //
	    // 0x5E:
	    //  - bits [3:0] = cantidad de electrodos habilitados
	    //
	    // 0x0C → habilita 12 electrodos (ELE0–ELE11)
	    // =========================================================
		pData[0] = 0x5E;
		pData[1] = 0x0C;
		if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 2, 1000) != HAL_OK) {
			return false;
		}

		// Recomendado si usás auto-config
		/* pData[0] = 0x7D;
		pData[1] = 0x9C;
		pData[2] = 0x65;
		pData[3] = 0x8C;
		if (HAL_I2C_Master_Transmit(&hi2c, MPR121_ADDR, pData, 4, 1000) != HAL_OK) {
			return false;
		} */

	    // =========================================================
	    // Listo: el módulo ya está midiendo y generando eventos
	    // =========================================================

	    return true;
}

