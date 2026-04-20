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
static void printVinMv(uint32_t Vin);
static void splitFourDigits(uint32_t value, uint8_t *thousands, uint8_t *hundreds, uint8_t *tens, uint8_t *units);

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
	    uint32_t vinMv;

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
	            if (API_MPR121_getKey(key_asterisk)) {
	                state = home;
	                API_LCD16x2_Clear();
	                API_LCD16x2_WriteStringAt(0, 0, homeMsg, strlen(homeMsg));
	            } else if (delayRead(&delay)) {
	                API_intSensors_readVoltMilliVolts(1000, &vinMv);
	                printVinMv(vinMv);
	            }
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
	splitFourDigits((uint32_t)absTenths, &hundreds, &tens, &units, &firstDecimal);

	printf("temp: ");
	if (temperatureTenths < 0) {
		printf("-");
	}
	printf("%d%d%d.%d\r\n", hundreds, tens, units, firstDecimal);

	if (temperatureTenths < 0) {
		lcdValue[0] = '-';
		lcdValue[1] = (char)('0' + hundreds);
		lcdValue[2] = (char)('0' + tens);
		lcdValue[3] = (char)('0' + units);
		lcdValue[4] = '.';
		lcdValue[5] = (char)('0' + firstDecimal);
		lcdValue[6] = ' ';
		lcdValue[7] = 'C';
		lcdValue[8] = '\0';
	} else {
		lcdValue[0] = (char)('0' + hundreds);
		lcdValue[1] = (char)('0' + tens);
		lcdValue[2] = (char)('0' + units);
		lcdValue[3] = '.';
		lcdValue[4] = (char)('0' + firstDecimal);
		lcdValue[5] = ' ';
		lcdValue[6] = 'C';
		lcdValue[7] = '\0';
	}

	API_LCD16x2_UpdateSecondRow(lcdValue, strlen(lcdValue));
}

static void printVinMv(uint32_t Vin) {
	uint8_t thousands;
	uint8_t hundreds;
	uint8_t tens;
	uint8_t units;
	char lcdValue[16];

	splitFourDigits(Vin, &thousands, &hundreds, &tens, &units);

	printf("vin: %d%d%d%d mV\r\n", thousands, hundreds, tens, units);

	lcdValue[0] = (char)('0' + thousands);
	lcdValue[1] = (char)('0' + hundreds);
	lcdValue[2] = (char)('0' + tens);
	lcdValue[3] = (char)('0' + units);
	lcdValue[4] = ' ';
	lcdValue[5] = 'm';
	lcdValue[6] = 'V';
	lcdValue[7] = '\0';
	API_LCD16x2_UpdateSecondRow(lcdValue, strlen(lcdValue));
}

static void splitFourDigits(uint32_t value, uint8_t *thousands, uint8_t *hundreds, uint8_t *tens, uint8_t *units) {
	if (thousands == NULL || hundreds == NULL || tens == NULL || units == NULL) {
		return;
	}

	*thousands = (uint8_t)((value / 1000U) % 10U);
	*hundreds = (uint8_t)((value / 100U) % 10U);
	*tens = (uint8_t)((value / 10U) % 10U);
	*units = (uint8_t)(value % 10U);
}
