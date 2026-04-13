/*
 * API_boardTemp_port.c
 *
 *  Created on: Apr 12, 2026
 *      Author: joaquin
 */

#include "API_boardTemp_port.h"
#include "stm32f4xx_ll_adc.h"

#define SLOPE 0.4
#define PEND -279

static bool_t initOk = false;
static ADC_HandleTypeDef hadc;
static ADC_ChannelConfTypeDef chConfig;

static bool_t handlerConfig(void);
static void channelConfig(void);
static float rawToCelsius(uint32_t rawValue);

bool_t API_boardTemp_port_init(void) {

	 __HAL_RCC_ADC1_CLK_ENABLE();

	 if (!handlerConfig()) {
		 initOk = false;
		 return false;
	 }
	 channelConfig();

	 if (HAL_ADC_ConfigChannel(&hadc, &chConfig) != HAL_OK) {
	 		return false;
	}

	initOk = true;
	return true;

}

bool_t API_boardTemp_port_startConvertion(void) {
	if (!initOk) {
		return false;
	}
	return HAL_ADC_Start(&hadc) == HAL_OK;
}

bool_t API_boardTemp_port_convertionReady(uint32_t timeout) {
	if (!initOk) {
		return false;
	}
	return HAL_ADC_PollForConversion(&hadc, timeout) == HAL_OK;
}

bool_t API_boardTemp_port_readRaw(uint32_t timeout, uint32_t *value) {
	if (value == NULL || !initOk) {
		return false;
	}

	if (HAL_ADC_Start(&hadc) != HAL_OK) {
		return false;
	}

	if (HAL_ADC_PollForConversion(&hadc, timeout) != HAL_OK) {
		return false;
	}

	*value = HAL_ADC_GetValue(&hadc);
	return true;
}

bool_t API_boardTemp_port_readCelsius(uint32_t timeout, float *temperatureC) {
	uint32_t rawValue;

	if (temperatureC == NULL) {
		return false;
	}

	if (!API_boardTemp_port_readRaw(timeout, &rawValue)) {
		return false;
	}

	*temperatureC = rawToCelsius(rawValue);
	return true;
}

/******************************************/
static bool_t handlerConfig(void) {
	hadc.Instance = ADC1;

	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc.Init.Resolution = ADC_RESOLUTION_12B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.ScanConvMode = DISABLE;
	hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	hadc.Init.ContinuousConvMode = ENABLE;
	hadc.Init.NbrOfConversion = 1;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.NbrOfDiscConversion = 0;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc.Init.DMAContinuousRequests = DISABLE;

	return HAL_ADC_Init(&hadc) == HAL_OK;
}

static void channelConfig(void) {
	chConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	chConfig.Rank = 1U;
	chConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	chConfig.Offset = 0U;
}

static float rawToCelsius(uint32_t rawValue) {
	float mVread = (float)(HAL_ADC_GetValue(&hadc)) / 4095 * 3300;
	return mVread * SLOPE + PEND;
}
