/// @file API_lcd_port.c
/// @version 1.0
/// @date 23/4/2023
/// @author Ing. Pavelek Israel
/// @title API de manejo del LCD de bajo nivel
/// @brief funciones lcd_port.c de bajo nivel



#include"API_Lcd_port.h"

static I2C_HandleTypeDef hi2c1;
static _Bool LCD_I2C_DeviceReady(void);
static _Bool LCD_I2C_Read_Port(uint8_t *port_value);

/**
  * @brief  GPIO_I2C configura los GPIO y clocks del I2C usado por el LCD.
  * @param  hi2c puntero al manejador I2C a inicializar
  * @return void
*/
void GPIO_I2C(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (hi2c == NULL) {
		return;
	}

	if (hi2c->Instance == I2C1) {
		__HAL_RCC_GPIOB_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_9;
//		GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		__HAL_RCC_I2C1_CLK_ENABLE();
	}
}

/**
  * @brief  LCD_HW_init inicializa la interfaz I2C de bajo nivel del LCD.
  * @param  void
  * @return LCD_OK si la inicializacion fue correcta, LCD_ERROR en caso contrario
*/
_Bool LCD_HW_init(void)
{
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	GPIO_I2C(&hi2c1);

	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		return LCD_ERROR;
	}

	if (LCD_I2C_DeviceReady() == LCD_ERROR) {
		return LCD_ERROR;
	}

	uint8_t port_value;
	if (LCD_I2C_Read_Port(&port_value) == LCD_ERROR) {
		return LCD_ERROR;
	}

	return LCD_OK;
}

/**
  * @brief  Verifica si el adaptador I2C del LCD responde con ACK.
  * @param  void
  * @return LCD_OK si el dispositivo responde, LCD_ERROR en caso contrario
  *
  * Nota: el backpack I2C del LCD no expone registros direccionables como una
  * memoria; por eso se usa una verificacion de presencia del esclavo.
*/
static _Bool LCD_I2C_DeviceReady(void)
{
	if (HAL_I2C_IsDeviceReady(&hi2c1, LCD_DIR << 1, 3, HAL_MAX_DELAY) != HAL_OK) {
		return LCD_ERROR;
	}

	return LCD_OK;
}

/**
  * @brief  Lee el byte del puerto del PCF8574 en modo READ.
  * @param  port_value puntero donde se guarda el valor leido
  * @return LCD_OK si la lectura fue correcta, LCD_ERROR en caso contrario
  *
  * El PCF8574 no tiene registros internos direccionables; la lectura devuelve
  * el estado actual de sus 8 lineas de puerto.
*/
static _Bool LCD_I2C_Read_Port(uint8_t *port_value)
{
	if (port_value == NULL) {
		return LCD_ERROR;
	}

	if (HAL_I2C_Master_Receive(&hi2c1, LCD_DIR << 1, port_value, 1, HAL_MAX_DELAY) != HAL_OK) {
		return LCD_ERROR;
	}

	return LCD_OK;
}

_Bool LCD_Read_Port(uint8_t *port_value)
{
	return LCD_I2C_Read_Port(port_value);
}


/**
  * @brief  LCD_Write_Byte Funciòn que saca el byte a escribir al LCD por I2C, en caso de trabajar de forma paralela reemplazar por
 * 			GPIO. Si hay un error queda el LED2 encendido y el micro en error
  * @param  uint8_t valor a sacar por el puerto
  * @return void
  * @author Ing. Pavelek Israel
  * @version 1.0
  * @date 16/4/2023
*/


void LCD_Write_Byte(uint8_t valor){
	if(HAL_I2C_Master_Transmit (&hi2c1, LCD_DIR<<1, &valor, sizeof(valor),HAL_MAX_DELAY) != HAL_OK)
    Error_Handler();

}


