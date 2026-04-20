/**
 * @file app.h
 * @brief Interfaz de la maquina de estados principal de la aplicacion.
 *
 * Este modulo declara la API publica de inicializacion y ejecucion del flujo
 * principal de la aplicacion, junto con los tipos asociados al estado general
 * y al registro de errores de inicializacion de los perifericos utilizados.
 */

#ifndef API_INC_APP_H_
#define API_INC_APP_H_

/********************************************************/
/* Includes */
#include "typedefs.h"

/********************************************************/
/* Declaracion de Tipos Publicos */
/**
 * @brief Banderas de error reportadas durante la inicializacion de la
 *        aplicacion.
 *
 * Cada valor representa una condicion de fallo asociada a un modulo
 * particular. Las banderas pueden combinarse para conservar el detalle de
 * multiples errores detectados durante `APP_init`.
 */
typedef enum {
    NO_ERROR = 0U,
    ERROR_MPR121 = (1U << 0),
    ERROR_LCD16x2 = (1U << 1),
    ERROR_INT_SENSORS = (1U << 2)
} error_e;

/**
 * @brief Estados de la maquina principal de la aplicacion.
 *
 * Define las pantallas y modos de operacion recorridos por el sistema durante
 * la navegacion por sensores, control de luces y tratamiento de errores.
 */
typedef enum {
    init = 0,
    home,
    analogSensors,
    temperatureSensors,
    voltimeterSensor,
    lights,
    lightsFix,
    lightsBlink,
    lightsBlinkSetFreq,
    error
} app_state_e;

/********************************************************/
/* Declaracion de Funciones Publicas */
/**
 * @brief Inicializa los modulos requeridos por la aplicacion.
 *
 * Configura los perifericos y servicios utilizados por la maquina de estados,
 * prepara la pantalla de bienvenida e inicializa los recursos locales
 * necesarios para la ejecucion del sistema.
 *
 * @return `true` si la inicializacion finalizo sin errores, `false` en caso
 *         contrario.
 */
bool_t APP_init();

/**
 * @brief Ejecuta un ciclo de la maquina de estados principal.
 *
 * Procesa eventos de entrada, actualiza la interfaz de usuario, atiende la
 * logica de sensores y de control del LED, y mantiene el avance general de la
 * aplicacion.
 *
 * @return `true` si el ciclo pudo ejecutarse, `false` si la aplicacion quedo
 *         inhibida por errores de inicializacion.
 */
bool_t APP_engine();

/**
 * @brief Devuelve el registro actual de errores de la aplicacion.
 *
 * @return Mascara de bits con las banderas de error detectadas durante la
 *         inicializacion.
 */
uint8_t APP_getError();

#endif /* API_INC_APP_H_ */
