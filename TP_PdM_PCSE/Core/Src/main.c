/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "API_led.h"
#include "API_delay.h"
#include "API_adc.h"

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
bool_t uartInit();

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	HAL_Init();
	SystemClock_Config();

	delay_t delay;
	delayInit(&delay, 500);

	uint8_t i = 0;

	led_t ledPlaquita;
	API_LED_Init(&ledPlaquita, LD2_GPIO_Port, LD2_Pin);

	adc_t tempSensor;
	API_adc_init(&tempSensor,
        ADC1,
        ADC_CHANNEL_TEMPSENSOR,
        ADC_SAMPLETIME_480CYCLES,
        GPIOA, 
        GPIO_PIN_0);
	API_adc_triggerConversion(&tempSensor, 1000);

	SystemClock_Config();

	uartInit();

    while (1)
    {
    	if (delayRead(&delay)) {
    		printf("Temp sensor: %ld \n", API_adc_readPolling(&tempSensor));
        API_adc_triggerConversion(&tempSensor, 1000);
//    		switch (i) {
//    		case 0:
//    			API_LED_SetMode(&ledPlaquita, FIX);
//    			API_LED_On(&ledPlaquita);
//    			printf("Led FIX\n");
//    			break;
//    		case 1:
//    			API_LED_SetMode(&ledPlaquita, BLINK);
//				API_LED_SetBlinkFreq(&ledPlaquita, 1);
//				printf("Led BLINK at 1Hz\n");
//				break;
//    		case 2:
//    			API_LED_SetMode(&ledPlaquita, BLINK);
//				API_LED_SetBlinkFreq(&ledPlaquita, 10);
//				printf("Led BLINK at 10Hz\n");
//				break;
//			default:
//				break;
//    		}
//
//    		i++;
//    		if (i >= 3) {
//    			i=0;
//    		}
		}

		API_LED_Engine(&ledPlaquita);
    }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


bool_t uartInit()
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;

	if (HAL_UART_Init(&huart2) != HAL_OK) {
		return false;
	} else {
		char buffer[200];
		sprintf(buffer,
		        "UART Configurated:\r\n"
		        "\tBaudRate: %lu\r\n"
		        "\tWordLength: %lu\r\n"
		        "\tStopBits: %lu\r\n"
		        "\tParity: %lu\r\n",
		        huart2.Init.BaudRate,
		        huart2.Init.WordLength,
		        huart2.Init.StopBits,
		        huart2.Init.Parity);

//		uartSendStringSize((uint8_t*)buffer, strlen(buffer));
		HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
	    return true;
	}
}

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
