/**
 * @file API_LCD16x2.h
 * @brief Interfaz de alto nivel para el manejo de un display LCD 16x2.
 *
 * Este módulo expone operaciones de inicialización, escritura y control sobre
 * un display LCD 16x2 conectado mediante un expansor I2C. La API ofrece
 * primitivas para posicionar el cursor, manipular el estado visual del display
 * y enviar caracteres o cadenas completas.
 */

#ifndef API_INC_API_LCD16X2_H_
#define API_INC_API_LCD16X2_H_

/********************************************************/
/* Includes */
#include "API_LCD16x2_port.h"

/********************************************************/
/* Declaracion de Tipos Publicos */
/**
 * @brief Representación por campos del byte de control enviado al expansor.
 *
 * Modela la estructura lógica del byte que combina las señales RS, enable,
 * retroiluminación y el nibble de datos a transmitir hacia el LCD.
 */
typedef struct tag_LCD_data_s
{
    uint8_t RS :1;
    uint8_t rsvd :1;
    uint8_t enable :1;
    uint8_t bakLight :1;
    uint8_t data :4;
} LCD_data_s;

/**
 * @brief Vista alternativa del byte de control del LCD.
 *
 * Permite acceder al mismo contenido tanto como estructura de campos de bits
 * como en formato crudo de un byte listo para transmitir por I2C.
 */
typedef union tag_LCD_data_u
{
    LCD_data_s data;
    uint8_t dataRaw;
} LCD_data_u;

/********************************************************/
/* Declaracion de Funciones Publicas */
/**
 * @brief Inicializa el display LCD 16x2 y su port I2C asociado.
 *
 * Configura el enlace I2C, ejecuta la secuencia de arranque del controlador LCD
 * y establece el estado inicial del display.
 *
 * @return `true` si la inicialización finalizó correctamente, `false` en caso
 *         contrario.
 */
bool_t API_LCD16x2_Init(void);

/**
 * @brief Envía un carácter al LCD en la posición actual del cursor.
 *
 * @param byte Carácter a transmitir al display.
 */
void API_LCD16x2_SendByte(char byte);

/**
 * @brief Envía una cadena de longitud fija al LCD.
 *
 * @param data Puntero a la cadena a transmitir.
 * @param size Cantidad de caracteres a enviar.
 */
void API_LCD16x2_SendString(const char *data, uint16_t size);

/**
 * @brief Limpia el contenido completo del display.
 */
void API_LCD16x2_Clear(void);

/**
 * @brief Retorna el cursor a la posición inicial del display.
 */
void API_LCD16x2_Home(void);

/**
 * @brief Habilita o deshabilita la visualización del display.
 *
 * @param visible `true` para mostrar el contenido, `false` para apagarlo.
 */
void API_LCD16x2_SetDisplayVisible(bool_t visible);

/**
 * @brief Habilita o deshabilita la visualización del cursor.
 *
 * @param visible `true` para mostrar el cursor, `false` para ocultarlo.
 */
void API_LCD16x2_SetCursorVisible(bool_t visible);

/**
 * @brief Habilita o deshabilita el parpadeo del cursor.
 *
 * @param enable `true` para activar el parpadeo, `false` para desactivarlo.
 */
void API_LCD16x2_SetCursorBlink(bool_t enable);

/**
 * @brief Posiciona el cursor en una fila y columna determinadas.
 *
 * @param row Fila destino del cursor.
 * @param col Columna destino del cursor.
 */
void API_LCD16x2_SetCursor(uint8_t row, uint8_t col);

/**
 * @brief Posiciona el cursor sobre la primera fila.
 *
 * @param pos Columna destino dentro de la primera fila.
 */
void API_LCD16x2_FirstRow(uint8_t pos);

/**
 * @brief Posiciona el cursor sobre la segunda fila.
 *
 * @param pos Columna destino dentro de la segunda fila.
 */
void API_LCD16x2_SecondRow(uint8_t pos);

/**
 * @brief Escribe un carácter en una posición específica del display.
 *
 * @param row Fila destino.
 * @param col Columna destino.
 * @param data Carácter a escribir.
 */
void API_LCD16x2_WriteCharAt(uint8_t row, uint8_t col, char data);

/**
 * @brief Escribe una cadena en una posición específica del display.
 *
 * @param row Fila inicial de escritura.
 * @param col Columna inicial de escritura.
 * @param data Puntero a la cadena a escribir.
 * @param size Cantidad de caracteres a transmitir.
 */
void API_LCD16x2_WriteStringAt(uint8_t row, uint8_t col, const char *data, uint16_t size);

/**
 * @brief Actualiza el contenido completo de la primera fila del display.
 *
 * Limpia previamente la fila con espacios y luego escribe la cadena indicada
 * desde la primera columna.
 *
 * @param data Puntero a la cadena a escribir en la primera fila.
 * @param size Cantidad de caracteres de la cadena a transmitir.
 */
void API_LCD16x2_UpdateFirstRow(const char *data, uint16_t size);

/**
 * @brief Actualiza el contenido completo de la segunda fila del display.
 *
 * Limpia previamente la fila con espacios y luego escribe la cadena indicada
 * desde la primera columna.
 *
 * @param data Puntero a la cadena a escribir en la segunda fila.
 * @param size Cantidad de caracteres de la cadena a transmitir.
 */
void API_LCD16x2_UpdateSecondRow(const char *data, uint16_t size);

/**
 * @brief Desplaza el cursor una posición hacia la izquierda o derecha.
 *
 * @param right `true` para desplazar hacia la derecha, `false` hacia la
 *              izquierda.
 */
void API_LCD16x2_ShiftCursor(bool_t right);

/**
 * @brief Desplaza el contenido completo del display.
 *
 * @param right `true` para desplazar hacia la derecha, `false` hacia la
 *              izquierda.
 */
void API_LCD16x2_ShiftDisplay(bool_t right);

/**
 * @brief Configura el modo de entrada del LCD.
 *
 * @param increment `true` para avanzar automáticamente la dirección de
 *                  escritura, `false` para decrementarla.
 * @param shiftDisplay `true` para desplazar el display durante la escritura,
 *                     `false` para mantenerlo fijo.
 */
void API_LCD16x2_SetEntryMode(bool_t increment, bool_t shiftDisplay);

/**
 * @brief Enciende o apaga la retroiluminación del LCD.
 *
 * @param on `true` para encender la retroiluminación, `false` para apagarla.
 */
void API_LCD16x2_Backlight(bool_t on);

/**
 * @brief Carga un texto antecedido por espacios para simular ingreso desde la
 *        derecha.
 *
 * @param data Puntero a la cadena a enviar.
 * @param size Cantidad de caracteres de la cadena a transmitir.
 */
void API_LCD16x2_LoadTextFromRight(const char *data, uint16_t size);

/**
 * @brief Envía al LCD un único dígito decimal en la posición actual del cursor.
 *
 * Convierte el valor numérico recibido a su carácter ASCII equivalente y lo
 * transmite al display. Esta función está pensada para dígitos individuales.
 *
 * @param number Dígito decimal a mostrar. Debe estar en el rango `0` a `9`.
 */
void API_LCD16x2_sendSingleNumber(uint8_t number);

#endif /* API_INC_API_LCD16X2_H_ */
