/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "spi.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "i2c.h"
//#include "i2s.h"


 


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
						

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void K4_fucntion();

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_2_Pin GPIO_PIN_13
#define LED_2_GPIO_Port GPIOC
#define LCD_RES_Pin GPIO_PIN_0
#define LCD_RES_GPIO_Port GPIOC
#define KEY_1_Pin GPIO_PIN_2
#define KEY_1_GPIO_Port GPIOC
#define KEY_1_EXTI_IRQn EXTI2_IRQn
#define KEY_2_Pin GPIO_PIN_3
#define KEY_2_GPIO_Port GPIOC
#define KEY_2_EXTI_IRQn EXTI3_IRQn
#define LCD_BLK_Pin GPIO_PIN_3
#define LCD_BLK_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOC
#define LCD_DC_Pin GPIO_PIN_5
#define LCD_DC_GPIO_Port GPIOC
#define LED_4_Pin GPIO_PIN_13
#define LED_4_GPIO_Port GPIOB
#define LED_3_Pin GPIO_PIN_14
#define LED_3_GPIO_Port GPIOB
#define LED_1_Pin GPIO_PIN_7
#define LED_1_GPIO_Port GPIOC
#define SDIO_CD_Pin GPIO_PIN_11
#define SDIO_CD_GPIO_Port GPIOA
#define KEY_3_Pin GPIO_PIN_4
#define KEY_3_GPIO_Port GPIOB
#define KEY_3_EXTI_IRQn EXTI4_IRQn
#define KEY_4_Pin GPIO_PIN_5
#define KEY_4_GPIO_Port GPIOB
#define KEY_4_EXTI_IRQn EXTI9_5_IRQn
#define Trigger_Pin GPIO_PIN_7
#define Trigger_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define	LED_1(x)	 if(x == 1)	\
									 HAL_GPIO_WritePin(GPIOC, LED_1_Pin, GPIO_PIN_RESET); \
									 else \
									 HAL_GPIO_WritePin(GPIOC, LED_1_Pin, GPIO_PIN_SET)						 
			
#define	LED_2(x)	 if(x == 1)	\
									 HAL_GPIO_WritePin(GPIOC, LED_2_Pin, GPIO_PIN_RESET); \
									 else \
									 HAL_GPIO_WritePin(GPIOC, LED_2_Pin, GPIO_PIN_SET)		
									 
#define	LED_3(x)	 if(x == 1)	\
									 HAL_GPIO_WritePin(GPIOB, LED_3_Pin, GPIO_PIN_RESET); \
									 else \
									 HAL_GPIO_WritePin(GPIOB, LED_3_Pin, GPIO_PIN_SET)		

#define	LED_4(x)	 if(x == 1)	\
									 HAL_GPIO_WritePin(GPIOB, LED_4_Pin, GPIO_PIN_RESET); \
									 else \
									 HAL_GPIO_WritePin(GPIOB, LED_4_Pin, GPIO_PIN_SET)											 

#define	LCD_BLK(x)		if(x == 0)	\
											HAL_GPIO_WritePin(GPIOA, LCD_BLK_Pin, GPIO_PIN_RESET); \
											else \
											HAL_GPIO_WritePin(GPIOA, LCD_BLK_Pin, GPIO_PIN_SET)		
									 
#define	LCD_DC(x)		if(x == 0)	\
											HAL_GPIO_WritePin(GPIOC, LCD_DC_Pin, GPIO_PIN_RESET); \
											else \
											HAL_GPIO_WritePin(GPIOC, LCD_DC_Pin, GPIO_PIN_SET)	

#define	LCD_RES(x)		if(x == 0)	\
											HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET); \
											else \
											HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET)	

#define	SPI1_CS(x)		if(x == 0)	\
											HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET); \
											else \
											HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET)	
											
#define	Trigger(x)		if(x == 0)	\
											HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_RESET); \
											else \
											HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_SET)	
											
											
											
#define	KEY_1					HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin)

#define	KEY_2					HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin)

#define	KEY_3					HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin)

#define	KEY_4					HAL_GPIO_ReadPin(KEY_4_GPIO_Port, KEY_4_Pin)											
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
