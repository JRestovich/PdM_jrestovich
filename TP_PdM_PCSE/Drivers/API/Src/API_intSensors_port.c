/**
 * @file API_intSensors_port.c
 * @brief Implementación de port para la lectura de sensores internos mediante ADC.
 *
 * Este archivo contiene la configuración del ADC, la secuencia de adquisición
 * de los canales internos del microcontrolador y las conversiones necesarias
 * para expresar los resultados en unidades de aplicación. La documentación de
 * la API pública se mantiene en `API_intSensors_port.h`, mientras que aquí se
 * documentan únicamente los elementos privados del módulo.
 */

#include "API_intSensors_port.h"
#include "stm32f4xx_ll_adc.h"

#define SLOPE 0.4   ///< Pendiente utilizada para convertir la lectura del sensor de temperatura a grados Celsius.
#define PEND -279   ///< Ordenada al origen utilizada para convertir la lectura del sensor de temperatura a grados Celsius.

static bool_t initOk = false;                  ///< Indica si el módulo de port fue inicializado correctamente.
static ADC_HandleTypeDef hadc;                 ///< Handler HAL asociado al periférico ADC utilizado por el módulo.
static ADC_ChannelConfTypeDef tempChConfig;    ///< Configuración del canal ADC correspondiente al sensor de temperatura interno.
static ADC_ChannelConfTypeDef voltChConfig;    ///< Configuración del canal ADC correspondiente a la referencia interna de tensión.

/**
 * @brief Configura los parámetros generales del periférico ADC.
 *
 * Inicializa la estructura `hadc` con la configuración base necesaria para
 * realizar una secuencia de dos conversiones sobre canales internos.
 *
 * @return `true` si la inicialización HAL del ADC fue exitosa, `false` en caso
 *         contrario.
 */
static bool_t handlerConfig(void);

/**
 * @brief Carga la configuración del canal ADC del sensor de temperatura interno.
 *
 * Prepara la estructura `tempChConfig` con el canal, rango y tiempo de muestreo
 * requeridos para el sensor de temperatura.
 */
static void tempChannelConfig(void);

/**
 * @brief Carga la configuración del canal ADC de referencia interna de tensión.
 *
 * Prepara la estructura `voltChConfig` con el canal, rango y tiempo de muestreo
 * requeridos para la medición de VREFINT.
 */
static void voltChannelConfig(void);

/**
 * @brief Aplica al ADC la secuencia de canales internos configurada.
 *
 * Programa en el periférico ADC los canales de temperatura y referencia
 * interna, respetando el orden de conversión definido en sus estructuras de
 * configuración.
 *
 * @return `true` si ambos canales fueron configurados correctamente, `false` en
 *         caso contrario.
 */
static bool_t applyChannelSequenceConfig(void);

/**
 * @brief Ejecuta una secuencia de lectura en crudo de temperatura y referencia
 *        interna.
 *
 * Lanza la conversión ADC de la secuencia configurada, espera la finalización de
 * cada muestra y devuelve los valores digitales de ambos canales internos.
 *
 * @param timeout Tiempo máximo de espera, en milisegundos, para cada conversión.
 * @param tempRaw Puntero donde se almacenará la lectura ADC del sensor de
 *                temperatura.
 * @param voltRaw Puntero donde se almacenará la lectura ADC de la referencia
 *                interna.
 *
 * @return `true` si la secuencia se ejecutó correctamente, `false` si ocurrió
 *         un error o los parámetros son inválidos.
 */
static bool_t readRawSequence(uint32_t timeout, uint32_t *tempRaw, uint32_t *voltRaw);

/**
 * @brief Convierte una lectura ADC del sensor de temperatura a grados Celsius.
 *
 * @param rawValue Valor ADC en crudo correspondiente al sensor de temperatura.
 *
 * @return Temperatura equivalente expresada en grados Celsius.
 */
static float rawToCelsius(uint32_t rawValue);

/**
 * @brief Convierte una lectura ADC de VREFINT a milivoltios.
 *
 * @param rawValue Valor ADC en crudo correspondiente a la referencia interna de
 *                 tensión.
 *
 * @return Tensión equivalente expresada en milivoltios.
 */
static uint32_t rawToMilliVolts(uint32_t rawValue);

bool_t API_intSensors_port_init(void) {
	__HAL_RCC_ADC1_CLK_ENABLE();

	if (!handlerConfig()) {
		initOk = false;
		return false;
	}

	tempChannelConfig();
	voltChannelConfig();

	if (!applyChannelSequenceConfig()) {
		initOk = false;
		return false;
	}

	initOk = true;
	return true;
}

bool_t API_intSensors_port_readTempRaw(uint32_t timeout, uint32_t *value) {
	uint32_t tempRaw;
	uint32_t voltRaw;

	if (!initOk || value == NULL) {
		return false;
	}

	if (!readRawSequence(timeout, &tempRaw, &voltRaw)) {
		return false;
	}

	*value = tempRaw;
	return true;
}

bool_t API_intSensors_port_readTempCelsius(uint32_t timeout, float *temperatureC) {
	uint32_t rawValue;

	if (!initOk || temperatureC == NULL) {
		return false;
	}

	if (!API_intSensors_port_readTempRaw(timeout, &rawValue)) {
		return false;
	}

	*temperatureC = rawToCelsius(rawValue);
	return true;
}

bool_t API_intSensors_port_readVoltRaw(uint32_t timeout, uint32_t *value) {
	uint32_t tempRaw;
	uint32_t voltRaw;

	if (!initOk || value == NULL) {
		return false;
	}

	if (!readRawSequence(timeout, &tempRaw, &voltRaw)) {
		return false;
	}

	*value = voltRaw;
	return true;
}

bool_t API_intSensors_port_readVoltMilliVolts(uint32_t timeout, uint32_t *voltageMv) {
	uint32_t rawValue;

	if (!initOk || voltageMv == NULL) {
		return false;
	}

	if (!API_intSensors_port_readVoltRaw(timeout, &rawValue)) {
		return false;
	}

	*voltageMv = rawToMilliVolts(rawValue);
	return true;
}

bool_t API_intSensors_port_readAllSensors(uint32_t timeout, float* temperatureC, uint32_t* voltageMv) {
	uint32_t rawValueTemp, rawValueV;

	if (!initOk || voltageMv == NULL) {
		return false;
	}

	if (!readRawSequence(timeout, &rawValueTemp, &rawValueV)) {
		return false;
	}

	*temperatureC = rawToCelsius(rawValueTemp);
	*voltageMv = rawToMilliVolts(rawValueV);
	return true;
}

static bool_t handlerConfig(void) {
	hadc.Instance = ADC1;

	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = ENABLE;
	hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.NbrOfConversion = 2;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.NbrOfDiscConversion = 0;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.DMAContinuousRequests = DISABLE;

	return HAL_ADC_Init(&hadc) == HAL_OK;
}

static void tempChannelConfig(void) {
	tempChConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	tempChConfig.Rank = 1U;
	tempChConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	tempChConfig.Offset = 0U;
}

static void voltChannelConfig(void) {
	voltChConfig.Channel = ADC_CHANNEL_VREFINT;
	voltChConfig.Rank = 2U;
	voltChConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	voltChConfig.Offset = 0U;
}

static bool_t applyChannelSequenceConfig(void) {
	if (HAL_ADC_ConfigChannel(&hadc, &tempChConfig) != HAL_OK) {
		return false;
	}

	if (HAL_ADC_ConfigChannel(&hadc, &voltChConfig) != HAL_OK) {
		return false;
	}

	return true;
}

static bool_t readRawSequence(uint32_t timeout, uint32_t *tempRaw, uint32_t *voltRaw) {
	if (!initOk || tempRaw == NULL || voltRaw == NULL) {
		return false;
	}

	if (!applyChannelSequenceConfig()) {
		return false;
	}

	if (HAL_ADC_Start(&hadc) != HAL_OK) {
		return false;
	}

	if (HAL_ADC_PollForConversion(&hadc, timeout) != HAL_OK) {
		return false;
	}
	*tempRaw = HAL_ADC_GetValue(&hadc);

	if (HAL_ADC_PollForConversion(&hadc, timeout) != HAL_OK) {
		return false;
	}
	*voltRaw = HAL_ADC_GetValue(&hadc);

	return HAL_ADC_Stop(&hadc) == HAL_OK;
}

static float rawToCelsius(uint32_t rawValue) {
	float mVread = (float)rawValue / 4095.0f * 3300.0f;
	return mVread * SLOPE + PEND;
}

static uint32_t rawToMilliVolts(uint32_t rawValue) {
	return __LL_ADC_CALC_VREFANALOG_VOLTAGE(rawValue, LL_ADC_RESOLUTION_12B);
}
