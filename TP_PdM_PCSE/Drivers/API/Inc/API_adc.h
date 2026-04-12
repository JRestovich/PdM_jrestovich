/*
 * API_analogSensor.h
 *
 *  Created on: Apr 11, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_ADC_H_
#define API_INC_API_ADC_H_

#include "stm32f4xx_hal.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	ADC_HandleTypeDef hadc;
	ADC_ChannelConfTypeDef channelConfig;
	uint32_t offset;
	uint32_t scale;
	uint32_t timeout;
} adc_t;

bool API_adc(adc_t *adc,
			 ADC_TypeDef* instance,
			 uint32_t channel,
			 uint32_t samplingTime);
void API_adc_triggerConversion(adc_t *adc, uint32_t timeout);
uint32_t API_adc_readPolling(adc_t *adc);

#endif /* API_INC_API_ADC_H_ */
