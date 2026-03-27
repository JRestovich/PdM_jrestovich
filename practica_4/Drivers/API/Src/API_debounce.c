/*
 * API_debounce.c
 *
 *  Created on: Mar 26, 2026
 *      Author: joaquin
 */
#include "API_debounce.h"
#include <stddef.h>
#include "stm32f4xx_hal.h"

#define ANTIBOUNCE_TIME_MS 40

static delay_t delay;
static debounceState_t state = BUTTON_UP;
static bool_t flanco = false;

void debounceFSM_init()
{
	//delayInit(&delay, ANTIBOUNCE_TIME_MS);
	state = BUTTON_UP;
}

void debounceFSM_update()
{
	switch(state) {
		case BUTTON_UP:
			if(!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) {
				state = BUTTON_FALLING;
				delayInit(&delay, ANTIBOUNCE_TIME_MS);
				flanco = true;
			}
			//buttonReleased();
			break;

		case BUTTON_FALLING:
			if (delayRead(&delay)) {
				if(!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) {
					state = BUTTON_DOWN;
				} else {
					state = BUTTON_UP;
				}
			}
			//buttonPressed();
			break;

		case BUTTON_DOWN:
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) {
				state = BUTTON_RAISING;
				delayInit(&delay, ANTIBOUNCE_TIME_MS);
			}
			//buttonPressed();
			break;

		case BUTTON_RAISING:
			if (delayRead(&delay)) {
				if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) {
					state = BUTTON_UP;
				} else {
					state = BUTTON_DOWN;
				}
			}
			//buttonReleased();
			break;
	}
}

bool_t readKey()
{
	bool_t ret = flanco;
	flanco = false;
	return ret;
}

/*
static void buttonPressed()
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
}

static void buttonReleased()
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
}*/

debounceState_t debounceFSM_getState()
{
	return state;
}


