/**
 * @file app.c
 * @brief Implementacion de la maquina de estados principal de la aplicacion.
 *
 * Este archivo integra la logica de navegacion entre pantallas, la lectura de
 * sensores internos, la gestion del teclado tactil y el control del LED de la
 * placa. La documentacion de la API publica se mantiene en `app.h`, mientras
 * que aqui se documentan unicamente los elementos privados del modulo.
 */

/********************************************************/
/* Includes */
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

/********************************************************/
/* Defines */
#define PORT_LED GPIOA                         ///< Puerto GPIO utilizado por el LED de usuario.
#define PIN_LED GPIO_PIN_5                     ///< Pin GPIO asociado al LED de usuario.
#define LCD_BUFFER_SIZE 16U                    ///< Longitud del buffer local usado para imprimir en el LCD.
#define WELCOME_DELAY_MS 5000U                 ///< Tiempo de permanencia de la pantalla de bienvenida.
#define ONE_SECOND_MS 1000U                    ///< Constante temporal de un segundo expresada en milisegundos.
#define FREQ_BUFFER_SIZE 3U                    ///< Cantidad maxima de digitos aceptados para la frecuencia de parpadeo.
#define DECIMAL_BASE 10U                       ///< Base decimal utilizada para conversiones numericas.
#define MAX_VALID_FREQ_HZ 200U                 ///< Frecuencia maxima valida para el parpadeo del LED.
#define TEMPERATURE_SCALE_FACTOR 10.0f         ///< Factor usado para obtener una cifra decimal de temperatura.
#define ASCII_ZERO '0'                         ///< Codigo ASCII del caracter `'0'`.
#define NEGATIVE_SIGN '-'                      ///< Signo utilizado para representar valores negativos.
#define DECIMAL_SEPARATOR '.'                  ///< Separador decimal mostrado en pantalla.
#define SPACE_CHAR ' '                         ///< Caracter espacio utilizado al formatear cadenas.
#define CELSIUS_UNIT 'C'                       ///< Unidad de temperatura mostrada en el LCD.
#define MILLI_UNIT 'm'                         ///< Prefijo de milivoltios mostrado en el LCD.
#define VOLT_UNIT 'V'                          ///< Unidad de tension mostrada en el LCD.

/********************************************************/
/* Variables privadas */
static const char welcomeMsg[]      = "Bienvenidos     "; ///< Texto mostrado durante la pantalla de bienvenida.
static const char homeMsg[]         = "1Sensores-2Luces"; ///< Texto base del menu principal.
static const char HomeSensors[]     = "1Temp-2Volt     "; ///< Texto del submenu de sensores analogicos.
static const char temperature[]     = "Temperatura:    "; ///< Etiqueta de la pantalla de temperatura.
static const char voltInput[]       = "Alimentacion:   "; ///< Etiqueta de la pantalla de tension de alimentacion.
static const char homeLights[]      = "1Fix-2Blink ";     ///< Texto del submenu de control de luces.
static const char ledOn[]           = "Led encendido   "; ///< Mensaje mostrado cuando el LED queda encendido fijo.
static const char ledOff[]          = "Led apagado     "; ///< Mensaje mostrado cuando el LED queda apagado.
static const char ledBlink[]        = "Led parpadeando "; ///< Mensaje mostrado cuando el LED entra en modo parpadeo.
static const char homeFix[]         = "1On-2off        "; ///< Texto del submenu de LED fijo.
static const char homeBlink[]       = "1On-2off-3Freq  "; ///< Texto del submenu de LED intermitente.
static const char blinkFreq[]       = "Frecuencia:     "; ///< Etiqueta para el ingreso de frecuencia de parpadeo.
static const char invalidFreq[]     = "Frec invalida   "; ///< Mensaje mostrado ante una frecuencia invalida.

static app_state_e state = init;                  ///< Estado actual de la maquina principal de la aplicacion.
static uint8_t errorFlag = NO_ERROR;              ///< Registro acumulado de errores de inicializacion.
static led_t led;                                 ///< Instancia del controlador del LED de usuario.
static delay_t delay;                             ///< Temporizador reutilizable para esperas no bloqueantes.

static uint8_t newFreq[FREQ_BUFFER_SIZE] = {0};   ///< Buffer temporal con los digitos de la nueva frecuencia.
static uint8_t newFreqIndex = 0;                  ///< Cantidad de digitos cargados en `newFreq`.

/********************************************************/
/* Declaracion de funciones privadas */
/**
 * @brief Convierte una temperatura en grados Celsius a una cadena para el LCD.
 *
 * Redondea el valor recibido a una cifra decimal, lo formatea en una cadena
 * compatible con el display y actualiza la segunda fila con el resultado.
 *
 * @param temperatureC Temperatura a mostrar en grados Celsius.
 */
static void printTemperatureDigits(float temperatureC);

/**
 * @brief Convierte una tension en milivoltios a una cadena para el LCD.
 *
 * Descompone el valor recibido en digitos decimales, arma la cadena con la
 * unidad correspondiente y actualiza la segunda fila del display.
 *
 * @param Vin Tension a mostrar en milivoltios.
 */
static void printVinMv(uint32_t Vin);

/**
 * @brief Descompone un numero decimal de hasta cuatro digitos.
 *
 * @param value Valor a separar en miles, centenas, decenas y unidades.
 * @param thousands Puntero donde se almacena el digito de los miles.
 * @param hundreds Puntero donde se almacena el digito de las centenas.
 * @param tens Puntero donde se almacena el digito de las decenas.
 * @param units Puntero donde se almacena el digito de las unidades.
 */
static void splitFourDigits(uint32_t value, uint8_t *thousands, uint8_t *hundreds, uint8_t *tens, uint8_t *units);

/**
 * @brief Convierte un arreglo de digitos decimales en un numero entero.
 *
 * Valida que cada posicion del arreglo contenga un digito decimal y construye
 * el valor final aplicando la base diez. Solo acepta frecuencias dentro del
 * rango valido definido por el modulo.
 *
 * @param ch Arreglo de digitos a convertir.
 * @param size Cantidad de elementos validos en `ch`.
 * @param value Puntero donde se almacena el numero convertido.
 *
 * @return `true` si la conversion fue valida y el numero quedo en rango,
 *         `false` en caso contrario.
 */
static bool_t array2Num(uint8_t *ch, uint8_t size, uint32_t *value);

/********************************************************/
/* Implementacion de funciones publicas */
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
                    state = lightsBlink;
                } else {
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

/********************************************************/
/* Implementacion de funciones privadas */
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
