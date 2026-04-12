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

typedef bool bool_t;

typedef struct {
	ADC_HandleTypeDef hadc;
	ADC_ChannelConfTypeDef channelConfig;
	uint32_t offset;
	uint32_t scale;
	uint32_t timeout;
	bool_t configOk;
} adc_t;

bool API_adc_init(adc_t *adc,
				ADC_TypeDef* instance,
				uint32_t channel,
				uint32_t samplingTime,
				GPIO_TypeDef* port,
				uint32_t pin);
void API_adc_triggerConversion(adc_t *adc, uint32_t timeout);
bool_t API_adc_convertionReady(adc_t *adc);
bool_t API_adc_readPolling(adc_t *adc, uint32_t* value);

#endif /* API_INC_API_ADC_H_ */
