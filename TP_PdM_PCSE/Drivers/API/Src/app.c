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

#define PORT_LED GPIOA
#define PIN_LED GPIO_PIN_5
#define LCD_BUFFER_SIZE 16U
#define WELCOME_DELAY_MS 5000U
#define ONE_SECOND_MS 1000U
#define FREQ_BUFFER_SIZE 3U
#define DECIMAL_BASE 10U
#define MAX_VALID_FREQ_HZ 200U
#define TEMPERATURE_SCALE_FACTOR 10.0f
#define ASCII_ZERO '0'
#define NEGATIVE_SIGN '-'
#define DECIMAL_SEPARATOR '.'
#define SPACE_CHAR ' '
#define CELSIUS_UNIT 'C'
#define MILLI_UNIT 'm'
#define VOLT_UNIT 'V'
//                                  = "0123456789ABCDEF";
static const char welcomeMsg[]      = "Bienvenidos     ";
static const char homeMsg[]         = "1Sensores-2Luces";
static const char HomeSensors[]     = "1Temp-2Volt     ";
static const char temperature[]     = "Temperatura:    ";
static const char voltInput[]       = "Alimentacion:   ";
static const char homeLights[]      = "1Fix-2Blink ";
static const char ledOn[]           = "Led encendido   ";
static const char ledOff[]          = "Led apagado     ";
static const char ledBlink[]        = "Led parpadeando ";
static const char homeFix[]         = "1On-2off        ";
static const char homeBlink[]       = "1On-2off-3Freq  ";
static const char blinkFreq[]       = "Frecuencia:     ";
static const char invalidFreq[]     = "Frec invalida   ";

static app_state_e state = init;
static uint8_t errorFlag = NO_ERROR;
static led_t led;
static delay_t delay;

static uint8_t newFreq[FREQ_BUFFER_SIZE] = {0};
static uint8_t newFreqIndex = 0;

static void printTemperatureDigits(float temperatureC);
static void printVinMv(uint32_t Vin);
static void splitFourDigits(uint32_t value, uint8_t *thousands, uint8_t *hundreds, uint8_t *tens, uint8_t *units);
static bool_t array2Num(uint8_t *ch, uint8_t size, uint32_t *value) ;

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
	API_LED_Init(&led, PORT_LED, PIN_LED);
	delayInit(&delay, WELCOME_DELAY_MS);
	API_LCD16x2_UpdateFirstRow(welcomeMsg, strlen(welcomeMsg));

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

    uint8_t key = 0U;

    uint32_t freqValue;

    if (touched) {
        printf("MPR121 value: %d \r\n", keysValue);
    }

    switch (state) {

        case init:
            if (delayRead(&delay)) {
                state = home;
                API_LCD16x2_UpdateFirstRow(homeMsg, strlen(homeMsg));
            }
            break;

        case home:
            if (!touched) {
                // Do nothing
            } else if (API_MPR121_getKey(key_1)) {
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(HomeSensors, strlen(HomeSensors));
                state = analogSensors;
            } else if (API_MPR121_getKey(key_2)) {
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(homeLights, strlen(homeLights));
                state = lights;
            }
            break;

        case analogSensors:
            if (!touched) {
                // Do nothing
            } else if (API_MPR121_getKey(key_1)) {
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(temperature, strlen(temperature));
                delayInit(&delay, ONE_SECOND_MS);
                state = temperatureSensors;
            } else if (API_MPR121_getKey(key_2)) {
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(voltInput, strlen(voltInput));
                state = voltimeterSensor;
                delayInit(&delay, ONE_SECOND_MS);
            } else if (API_MPR121_getKey(key_asterisk)) {
                state = home;
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(homeMsg, strlen(homeMsg));
            }
            break;

        case temperatureSensors:
            if (delayRead(&delay)) {
                API_intSensors_readTempCelsius(ONE_SECOND_MS, &sensorRead);
                printTemperatureDigits(sensorRead);
            } else if (!touched) {
                // Do nothing
            } else if (API_MPR121_getKey(key_asterisk)) {
                state = home;
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(homeMsg, strlen(homeMsg));
            }
            break;

        case voltimeterSensor:
            if (delayRead(&delay)) {
                API_intSensors_readVoltMilliVolts(ONE_SECOND_MS, &vinMv);
                printVinMv(vinMv);
            } else if (!touched) {
                // Do nothing
            } else if (API_MPR121_getKey(key_asterisk)) {
                state = home;
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(homeMsg, strlen(homeMsg));
            }
            break;

        case lights:
            if (!touched) {
                // Do nothing
            } else if (API_MPR121_getKey(key_asterisk)) {
                state = home;
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(homeMsg, strlen(homeMsg));
            } else if (API_MPR121_getKey(key_1)) {
                state = lightsFix;
                API_LCD16x2_UpdateFirstRow(homeFix, strlen(homeFix));
                API_LED_On(&led);
            } else if (API_MPR121_getKey(key_2)) {
                state = lightsBlink;
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(homeBlink, strlen(homeBlink));
            }
            break;

        case lightsFix:
            if (!touched) {
                // Do nothing
            } else if (API_MPR121_getKey(key_asterisk)) {
                state = home;
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(homeMsg, strlen(homeMsg));
            }  else if (API_MPR121_getKey(key_1)) {
                API_LCD16x2_UpdateSecondRow(ledOn, strlen(ledOn));
                API_LED_On(&led);
            } else if (API_MPR121_getKey(key_2)) {
                API_LCD16x2_UpdateSecondRow(ledOff, strlen(ledOff));
                API_LED_Off(&led);
            }
            break;

        case lightsBlink:
            if (!touched) {
                // Do nothing
            } else if (API_MPR121_getKey(key_asterisk)) {
                state = home;
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(homeMsg, strlen(homeMsg));
            }  else if (API_MPR121_getKey(key_1)) {
                API_LCD16x2_UpdateSecondRow(ledBlink, strlen(ledBlink));
                API_LED_SetMode(&led, BLINK);
            }  else if (API_MPR121_getKey(key_2)) {
                API_LCD16x2_UpdateSecondRow(ledOff, strlen(ledOff));
                API_LED_SetMode(&led, FIX);
            }  else if (API_MPR121_getKey(key_3)) {
                state = lightsBlinkSetFreq;
                newFreqIndex = 0;
                memset(newFreq, 0, sizeof(newFreq)); ///< clear array
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(blinkFreq, strlen(blinkFreq));
                API_LCD16x2_UpdateSecondRow("", 0U);
            }
            break;

        case lightsBlinkSetFreq:
            if (!touched) {
                // Do nothing
            } else if (API_MPR121_getKey(key_asterisk)) {
                state = home;
                API_LCD16x2_Clear();
                API_LCD16x2_UpdateFirstRow(homeMsg, strlen(homeMsg));
            }  else if (API_MPR121_getKey(key_hashtag)) {
                if (array2Num(&newFreq[0], newFreqIndex, &freqValue)) {
                    API_LED_SetBlinkFreq(&led, freqValue);
                    printf("new freq: %lu \r\n", freqValue);
                    state = lightsBlink;
                } else {
                    printf("freq not set: %lu \r\n", freqValue);
                    state = error;
                    delayInit(&delay, ONE_SECOND_MS);
                    API_LCD16x2_Clear();
                    API_LCD16x2_UpdateFirstRow(invalidFreq, strlen(invalidFreq));
                }
            } else if (API_MPR121_getSingleKey(&key)) {
                API_LCD16x2_sendSingleNumber(key);
                if (newFreqIndex < FREQ_BUFFER_SIZE) {
                    newFreq[newFreqIndex] = key;
                    newFreqIndex++;
                } else {
                    state = error;
                    delayInit(&delay, ONE_SECOND_MS);
                    API_LCD16x2_Clear();
                    API_LCD16x2_UpdateFirstRow(invalidFreq, strlen(invalidFreq));
                }
            }
            break;

        case error:
            if (delayRead(&delay)) {
                state = home;
                API_LCD16x2_UpdateFirstRow(homeMsg, strlen(homeMsg));
            }
            break;

        default:
            break;
    }

    API_LED_Engine(&led);

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
	char lcdValue[LCD_BUFFER_SIZE];

	temperatureTenths = (int32_t)lroundf(temperatureC * TEMPERATURE_SCALE_FACTOR);
	absTenths = abs(temperatureTenths);
	splitFourDigits((uint32_t)absTenths, &hundreds, &tens, &units, &firstDecimal);

	printf("temp: ");
	if (temperatureTenths < 0) {
		printf("%c", NEGATIVE_SIGN);
	}
	printf("%d%d%d.%d\r\n", hundreds, tens, units, firstDecimal);

	if (temperatureTenths < 0) {
		lcdValue[0] = NEGATIVE_SIGN;
		lcdValue[1] = (char)(ASCII_ZERO + hundreds);
		lcdValue[2] = (char)(ASCII_ZERO + tens);
		lcdValue[3] = (char)(ASCII_ZERO + units);
		lcdValue[4] = DECIMAL_SEPARATOR;
		lcdValue[5] = (char)(ASCII_ZERO + firstDecimal);
		lcdValue[6] = SPACE_CHAR;
		lcdValue[7] = CELSIUS_UNIT;
		lcdValue[8] = '\0';
	} else {
		lcdValue[0] = (char)(ASCII_ZERO + hundreds);
		lcdValue[1] = (char)(ASCII_ZERO + tens);
		lcdValue[2] = (char)(ASCII_ZERO + units);
		lcdValue[3] = DECIMAL_SEPARATOR;
		lcdValue[4] = (char)(ASCII_ZERO + firstDecimal);
		lcdValue[5] = SPACE_CHAR;
		lcdValue[6] = CELSIUS_UNIT;
		lcdValue[7] = '\0';
	}

	API_LCD16x2_UpdateSecondRow(lcdValue, strlen(lcdValue));
}

static void printVinMv(uint32_t Vin) {
	uint8_t thousands;
	uint8_t hundreds;
	uint8_t tens;
	uint8_t units;
	char lcdValue[LCD_BUFFER_SIZE];

	splitFourDigits(Vin, &thousands, &hundreds, &tens, &units);

	printf("vin: %d%d%d%d mV\r\n", thousands, hundreds, tens, units);

	lcdValue[0] = (char)(ASCII_ZERO + thousands);
	lcdValue[1] = (char)(ASCII_ZERO + hundreds);
	lcdValue[2] = (char)(ASCII_ZERO + tens);
	lcdValue[3] = (char)(ASCII_ZERO + units);
	lcdValue[4] = SPACE_CHAR;
	lcdValue[5] = MILLI_UNIT;
	lcdValue[6] = VOLT_UNIT;
	lcdValue[7] = '\0';
	API_LCD16x2_UpdateSecondRow(lcdValue, strlen(lcdValue));
}

static void splitFourDigits(uint32_t value, uint8_t *thousands, uint8_t *hundreds, uint8_t *tens, uint8_t *units) {
	if (thousands == NULL || hundreds == NULL || tens == NULL || units == NULL) {
		return;
	}

	*thousands = (uint8_t)((value / (DECIMAL_BASE * DECIMAL_BASE * DECIMAL_BASE)) % DECIMAL_BASE);
	*hundreds = (uint8_t)((value / (DECIMAL_BASE * DECIMAL_BASE)) % DECIMAL_BASE);
	*tens = (uint8_t)((value / DECIMAL_BASE) % DECIMAL_BASE);
	*units = (uint8_t)(value % DECIMAL_BASE);
}

static bool_t array2Num(uint8_t *ch, uint8_t size, uint32_t *value) {
    uint8_t i;
    uint16_t convertedValue = 0U;

    if ((ch == NULL) || (value == NULL) || (size == 0U)) {
        return false;
    }

    for (i = 0; i < size; i++) {
        if (ch[i] >= DECIMAL_BASE) {
            return false;
        }
        convertedValue = (uint16_t)((convertedValue * DECIMAL_BASE) + ch[i]);
    }

    *value = convertedValue;
    return (*value > 0U) && (*value < MAX_VALID_FREQ_HZ);
}
