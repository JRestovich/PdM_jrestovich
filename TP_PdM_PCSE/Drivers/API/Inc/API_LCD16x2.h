/*
 * API_LCD16x2.h
 *
 *  Created on: Apr 17, 2026
 *      Author: joaquin
 */

#ifndef API_INC_API_LCD16X2_H_
#define API_INC_API_LCD16X2_H_

#include "API_LCD16x2_port.h"

//Bits de ENABLE y BACK LIGHT
#define EN (1<<2)
#define BL (1<<3)

//SEÑALES DE CONTROL

#define CLR_LCD 1
#define RETURN_HOME (1<<1)
#define ENTRY_MODE (1<<2)
#define DISPLAY_CONTROL (1<<3)
#define CURSOR_DISPLAY_SHIFT (1<<4)
#define FUNTION_SET (1<<5)
#define SET_CGRAM (1<<6)
#define SET_DDRAM (1<<7)
/************************************/
#define DISPLAY_ON (1<<2)
#define CURSOR_ON (1<<1)
#define CURSOR_BLINK 1
#define AUTOINCREMENT (1<<1)
#define _4BIT_MODE 0x28

typedef struct tag_LCD_data_s
{
    uint8_t data :4;
    uint8_t bakLight :1;
    uint8_t enable :1;
    uint8_t rsvd :1;
    uint8_t type :1;
} LCD_data_s;

typedef union tag_LCD_data_u
{
    LCD_data_s data;
    uint8_t raw;
} LCD_data_u;



bool_t API_LCD16x2_Init(void);

#endif /* API_INC_API_LCD16X2_H_ */
