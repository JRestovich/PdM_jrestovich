/*
 * app.h
 *
 *  Created on: Apr 20, 2026
 *      Author: joaquin
 */

#ifndef API_INC_APP_H_
#define API_INC_APP_H_

#include "typedefs.h"

typedef enum {
    NO_ERROR = 0U,
    ERROR_MPR121 = (1U << 0),
    ERROR_LCD16x2 = (1U << 1),
    ERROR_INT_SENSORS = (1U << 2)
} error_e;

typedef enum {
    init = 0,
    home,
    analogSensors,
    temperatureSensors,
    voltimeterSensor,
    lights,
    lightsFixOn,
    lightsFixOff,
    lightsBlink,
    lightsBlinkSetFreq,
    error
} app_state_e;

bool_t APP_init();
bool_t APP_engine();
uint8_t APP_getError();

#endif /* API_INC_APP_H_ */
