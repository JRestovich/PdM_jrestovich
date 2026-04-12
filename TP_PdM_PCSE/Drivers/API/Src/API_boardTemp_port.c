/*
 * API_boardTemp_port.c
 *
 *  Created on: Apr 12, 2026
 *      Author: joaquin
 */

#include "API_boardTemp_port.h"

static bool_t initOk = false;
static ADC_HandleTypeDef hadc;
static ADC_ChannelConfTypeDef chConfig;

static bool_t handlerConfig(void);
static void channelConfig(void);
static void initAdcHw(void);

//ADC1,
//ADC_CHANNEL_TEMPSENSOR,
//ADC_SAMPLETIME_480CYCLES,
//GPIOA,
//GPIO_PIN_0

bool_t API_boardTemp_port_init(void) {

	 __HAL_RCC_ADC1_CLK_ENABLE();

	 if (!handlerConfig()) {
		 initOk = false;
		 return false;
	 }
	 channelConfig();
	 initAdcHw();

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

static void initAdcHw(void) {
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
