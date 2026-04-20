/*
 * app.c
 *
 *  Created on: Apr 20, 2026
 *      Author: joaquin
 */
#include "app.h"
#include <string.h>
#include "API_MPR121.h"
#include "API_LCD16x2.h"
#include "API_intSensors.h"
#include "API_delay.h"
#include "API_led.h"

#define LD2_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
//                                  = "0123456789ABCDEF";
static const char welcomeMsg[]      = "Bienvenidos";
static const char homeMsg[]         = "1Sensores-2Luces";
static const char HomeSensors[]     = "1Temp-2Volt";
static const char homeLights[]      = "1On-2Off-3Blink";

static app_state_e state = init;
static uint8_t errorFlag = NO_ERROR;
static led_t* led;
static delay_t delay;

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
	delayInit(&delay, 10000);
	API_LCD16x2_WriteStringAt(0, 0, welcomeMsg, strlen(welcomeMsg));

	return errorFlag == NO_ERROR;
}

bool_t APP_engine() {
    if (errorFlag != NO_ERROR) {
        return false;
    }
    uint16_t keysValue;
    bool_t touched = API_MPR121_readKeys(&keysValue);

    switch (state) {
        case init:
            if (delayRead(&delay)) {
                state = home;
                API_LCD16x2_WriteStringAt(0, 0, homeMsg, strlen(homeMsg));
            }
            break;

        case home:
            if (touched) {
                printf("MPR121 vlaue: %d", keysValue);
               if (keysValue == key_1) {
                   API_LCD16x2_WriteStringAt(0, 0, HomeSensors, strlen(HomeSensors));
               } else if (keysValue == key_2) {
                   API_LCD16x2_WriteStringAt(0, 0, homeLights, strlen(homeLights));
               }
            }

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

uint8_t APP_getError() {
    return errorFlag;
}
