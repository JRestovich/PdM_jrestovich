/*
 * API_intSensors_port.c
 *
 *  Created on: Apr 12, 2026
 *      Author: joaquin
 */

#include "API_intSensors_port.h"
#include "stm32f4xx_ll_adc.h"

#define SLOPE 0.4
#define PEND -279

static bool_t initOk = false;
static ADC_HandleTypeDef hadc;
static ADC_ChannelConfTypeDef tempChConfig;
static ADC_ChannelConfTypeDef voltChConfig;

static bool_t handlerConfig(void);
static void tempChannelConfig(void);
static void voltChannelConfig(void);
static bool_t applyChannelSequenceConfig(void);
static bool_t readRawSequence(uint32_t timeout, uint32_t *tempRaw, uint32_t *voltRaw);
static float rawToCelsius(uint32_t rawValue);
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
