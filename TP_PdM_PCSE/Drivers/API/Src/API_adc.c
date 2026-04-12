#include "API_adc.h"

void initAdcClock(ADC_HandleTypeDef* hadc);
void initAdcHw(ADC_HandleTypeDef* hadc, uint32_t channel, GPIO_TypeDef* port, uint32_t pin);

bool API_adc_init(adc_t *adc,
				ADC_TypeDef* instance,
				uint32_t channel,
				uint32_t samplingTime,
				GPIO_TypeDef* port,
				uint32_t pin) {
	if (adc == NULL || instance == NULL) {
		return false;
	}

	adc->hadc.Instance = instance;
	initAdcHw(&adc->hadc, channel, port, pin);

	initAdcClock(&adc->hadc);

	adc->hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	adc->hadc.Init.Resolution = ADC_RESOLUTION_12B;
	adc->hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	adc->hadc.Init.ScanConvMode = DISABLE;
	adc->hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	adc->hadc.Init.ContinuousConvMode = ENABLE;
	adc->hadc.Init.NbrOfConversion = 1;
	adc->hadc.Init.DiscontinuousConvMode = DISABLE;
	adc->hadc.Init.NbrOfDiscConversion = 0;
	adc->hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	adc->hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	adc->hadc.Init.DMAContinuousRequests = DISABLE;
	adc->timeout = HAL_MAX_DELAY;

	if (HAL_ADC_Init(&adc->hadc) != HAL_OK) {
		return false;
	}

	adc->channelConfig.Channel = channel;
	adc->channelConfig.Rank = 1U;
	adc->channelConfig.SamplingTime = samplingTime;
	adc->channelConfig.Offset = 0U;

	return (HAL_ADC_ConfigChannel(&adc->hadc, &adc->channelConfig) == HAL_OK);
}

void API_adc_triggerConversion(adc_t *adc, uint32_t timeout) {
	if (adc == NULL) {
		return;
	}

	adc->timeout = timeout;
}

uint32_t API_adc_readPolling(adc_t *adc) {
	if (adc == NULL) {
		return 0U;
	}

	if (HAL_ADC_PollForConversion(&adc->hadc, adc->timeout) != HAL_OK) {
		return 0U;
	}

	return HAL_ADC_GetValue(&adc->hadc);
}

void initAdcHw(ADC_HandleTypeDef* hadc, uint32_t channel, GPIO_TypeDef* port, uint32_t pin) {
    GPIO_InitTypeDef GPIO_InitStruct;

    if (port == GPIOA) {
    	__HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (port == GPIOB) {
    	__HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (port == GPIOC) {
    	__HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (port == GPIOD) {
    	__HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (port == GPIOE) {
    	__HAL_RCC_GPIOE_CLK_ENABLE();
    } else if (port == GPIOF) {
    	__HAL_RCC_GPIOF_CLK_ENABLE();
    } else if (port == GPIOG) {
    	__HAL_RCC_GPIOG_CLK_ENABLE();
    } else if (port == GPIOH) {
    	__HAL_RCC_GPIOH_CLK_ENABLE();
    }

    GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(port, &GPIO_InitStruct);
}

void initAdcClock(ADC_HandleTypeDef* hadc) {
    if(hadc->Instance == ADC1) {
        __HAL_RCC_ADC1_CLK_ENABLE();
    } else if(hadc->Instance == ADC2) {
        __HAL_RCC_ADC2_CLK_ENABLE();
    } else if(hadc->Instance == ADC3) {
    	__HAL_RCC_ADC3_CLK_ENABLE();
    }
}
