/*
 * app.c
 *
 *  Created on: Apr 20, 2026
 *      Author: joaquin
 */
#include "app.h"
#include "API_MPR121.h"
#include "API_LCD16x2.h"
#include "API_intSensors.h"
#include "API_delay.h"
#include "API_led.h"

#define LD2_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5

static app_state_e state = init;
static uint8_t errorFlag = NO_ERROR;
static led_t* led;

bool_t APP_init() {
	if (!API_MPR121_init()) {
		printf("Error al inicializar MPR121\r\n");
		errorFlag |= ERROR_MPR121;
	}
	if (!API_LCD16x2_Init()) {
		printf("Error al inicializar MPR121\r\n");
		errorFlag |= ERROR_LCD16x2;
	}
	if (!API_intSensors_init()) {
        printf("Error al inicializar MPR121\r\n");
        errorFlag |= ERROR_INT_SENSORS;
    }
	API_LED_Init(led, LD2_GPIO_Port, LD2_Pin);

	return errorFlag == NO_ERROR;
}

bool_t APP_engine() {
    if (errorFlag != NO_ERROR) {
        return false;
    }
    uint16_t keysValue;
    if (API_MPR121_readKeys(&keysValue)) {

    }
    switch (state) {
        case init:
            break;

        case home:
            break;

        case analogSensors:
            break;

        case temperatureSensors:
            break;

        case voltimeterSensor:
            break;

        case lights:
            break;

        case lightsFixOn:
            break;

        case lightsFixOff:
            break;

        case lightsBlink:
            break;

        case lightsBlinkSetFreq:
            break;

        case error:
            break;

        default:
            break;
    }

    return true;
}
