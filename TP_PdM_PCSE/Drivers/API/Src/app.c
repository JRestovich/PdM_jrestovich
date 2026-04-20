/*
 * app.c
 *
 *  Created on: Apr 20, 2026
 *      Author: joaquin
 */
#include "app.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "API_MPR121.h"
#include "API_LCD16x2.h"
#include "API_intSensors.h"
#include "API_delay.h"
#include "API_led.h"

#define LD2_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
//                                  = "0123456789ABCDEF";
static const char whiteLine[]       = "                ";
static const char welcomeMsg[]      = "Bienvenidos";
static const char homeMsg[]         = "1Sensores-2Luces";
static const char HomeSensors[]     = "1Temp-2Volt";
static const char homeLights[]      = "1On-2Off-3Blink";
static const char temperature[]     = "Temperatura:";
static const char voltInput[]       = "Alimentacion: ";

static app_state_e state = init;
static uint8_t errorFlag = NO_ERROR;
static led_t* led;
static delay_t delay;

static void printTemperatureDigits(float temperatureC);

bool_t APP_init() {
	if (!API_MPR121_init()) {
		printf("Error al inicializar MPR121\r\n");
		errorFlag |= ERROR_MPR121;
	}
	if (!API_LCD16x2_Init()) {
		printf("Error al inicializar LCD\r\n");
		errorFlag |= ERROR_LCD16x2;
	}
	if (!API_intSensors_init()) {
        printf("Error al inicializar Sensores internos\r\n");
        errorFlag |= ERROR_INT_SENSORS;
    }
	API_LED_Init(led, LD2_GPIO_Port, LD2_Pin);
	delayInit(&delay, 5000);
	API_LCD16x2_WriteStringAt(0, 0, welcomeMsg, strlen(welcomeMsg));

	return errorFlag == NO_ERROR;
}

bool_t APP_engine() {
    if (errorFlag != NO_ERROR) {
        return false;
    }
    float sensorRead;

    uint16_t keysValue;
    bool_t touched = API_MPR121_readKeys(&keysValue);
    if (touched) {
        printf("MPR121 value: %d \r\n", keysValue);
    }

    switch (state) {
        case init:
            if (delayRead(&delay)) {
                state = home;
                API_LCD16x2_WriteStringAt(0, 0, homeMsg, strlen(homeMsg));
            }
            break;

        case home:
            if (!touched) {
                // Do nothing
            } else if (API_MPR121_getKey(key_1)) {
                API_LCD16x2_Clear();
                API_LCD16x2_WriteStringAt(0, 0, HomeSensors, strlen(HomeSensors));
                state = analogSensors;
            } else if (API_MPR121_getKey(key_2)) {
                API_LCD16x2_Clear();
                API_LCD16x2_WriteStringAt(0, 0, homeLights, strlen(homeLights));
                state = lights;
            }
            break;

        case analogSensors:
            if (!touched) {
                // Do nothing
            } else if (API_MPR121_getKey(key_1)) {
                API_LCD16x2_Clear();
                API_LCD16x2_WriteStringAt(0, 0, temperature, strlen(temperature));
                delayInit(&delay, 1000);
                state = temperatureSensors;
            } else if (API_MPR121_getKey(key_2)) {
                API_LCD16x2_Clear();
                API_LCD16x2_WriteStringAt(0, 0, voltInput, strlen(voltInput));
                state = voltimeterSensor;
                delayInit(&delay, 1000);
            } else if (API_MPR121_getKey(key_asterisk)) {
                state = home;
                API_LCD16x2_Clear();
                API_LCD16x2_WriteStringAt(0, 0, homeMsg, strlen(homeMsg));
            }
            break;

	        case temperatureSensors:
	            if (API_MPR121_getKey(key_asterisk)) {
	                state = home;
	                API_LCD16x2_Clear();
	                API_LCD16x2_WriteStringAt(0, 0, homeMsg, strlen(homeMsg));
	            } else if (delayRead(&delay)) {
	                API_intSensors_readTempCelsius(1000, &sensorRead);
	                printTemperatureDigits(sensorRead);
	            }
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

static void printTemperatureDigits(float temperatureC) {
	int32_t temperatureTenths;
	int32_t absTenths;
	uint8_t hundreds;
	uint8_t tens;
	uint8_t units;
	uint8_t firstDecimal;
	char lcdValue[16];

	temperatureTenths = (int32_t)lroundf(temperatureC * 10.0f);
	absTenths = abs(temperatureTenths);

	hundreds = (uint8_t)((absTenths / 1000U) % 10U);
	tens = (uint8_t)((absTenths / 100U) % 10U);
	units = (uint8_t)((absTenths / 10U) % 10U);
	firstDecimal = (uint8_t)(absTenths % 10U);

	printf("temp: ");
	if (temperatureTenths < 0) {
		printf("-");
	}
	printf("%d%d%d.%d\r\n", hundreds, tens, units, firstDecimal);

	if (temperatureTenths < 0) {
		snprintf(lcdValue, sizeof(lcdValue), "-%d%d%d.%d C", hundreds, tens, units, firstDecimal);
	} else {
		snprintf(lcdValue, sizeof(lcdValue), "%d%d%d.%d C", hundreds, tens, units, firstDecimal);
	}

	API_LCD16x2_WriteStringAt(1, 0, whiteLine, strlen(whiteLine));
	API_LCD16x2_WriteStringAt(1, 0, lcdValue, strlen(lcdValue));
}
