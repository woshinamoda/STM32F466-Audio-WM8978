/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "dac.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "i2s.h"
#include "quadspi.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "app_touchgfx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "system_st7735.h"
#include "system_led.h"
#include "system_key.h"
#include "system_i2s.h"  
	
	
#include "audio_wm8978_I2C.h"
#include "system_exfuns.h"
#include "system_malloc.h"	  
#include "system_wavplay.h"  
#include "system_fem.h"

#include "output_aep.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//调制-XX-HZ.wav
//click-XX-HZ.wav		XX:25\30\35\40\45
uint8_t Click_15HZ[] = {"0:/MUSIC/2倍Click15HZ.wav"};
uint8_t Click_30HZ[] = {"0:/MUSIC/2倍Click30HZ.wav"};
uint8_t Click_35HZ[] = {"0:/MUSIC/2倍Click35HZ.wav"};
uint8_t Click_40HZ[] = {"0:/MUSIC/2倍Click40HZ.wav"};
uint8_t Click_45HZ[] = {"0:/MUSIC/2倍Click45HZ.wav"};

uint8_t sine_40HZ[] = {"0:/MUSIC/正弦_40.wav"};
uint8_t sine_45HZ[] = {"0:/MUSIC/正弦_45.wav"};
uint8_t sine_50HZ[] = {"0:/MUSIC/正弦_50.wav"};
uint8_t sine_55HZ[] = {"0:/MUSIC/正弦_55.wav"};
uint8_t sine_60HZ[] = {"0:/MUSIC/正弦_60.wav"};


uint8_t  res;
uint32_t File_Size;								//读取到的歌曲文件大小!
uint8_t  volum = 20;							//用于显示的音量			0 - 100
uint16_t act_volum;								//真实的DAC音量等级 	0 - 61


uint8_t play_cnt = 1;							//       1  2  3  4  5   6  7  8  9  10	|			 11  12  13  14  15 
																	//click 15 30 35 40 45
																	//正弦                   40 45 50 55 60
												


uint8_t *play_name;
uint8_t K4_set_OK = 0;

extern __wavctrl wavctrl;			  	//WAV控制结构体
extern uint8_t PlayingSig;								
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_DMA_Init();
  MX_CRC_Init();
  MX_USART1_UART_Init();
  MX_TIM7_Init();
  MX_I2S2_Init();
  MX_I2C1_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_QUADSPI_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_TouchGFX_Init();
  /* USER CODE BEGIN 2 */
//	uPrintf("FatFs SD-Card init OK\n\r");
//	HAL_Delay(100);

	//HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
	uPrintf("system TIM init OK\n\r");
	
	Lcd_Init();
	Lcd_Clear(BLACK);	
	LCD_BLK(1);
	uPrintf("LCD init OK\n\r");	

//	ASSR_init(click_mode);
//	uPrintf("ASSR init OK\n\r");		
	

//	act_volum = volum * 61 / 100;
//	WM8978_Init(act_volum);
//	f_mount(&SDFatFS,"0:",1); 			//挂载SD卡 

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//	play_name = Click_15HZ;
//	WM8978_SetMute(1); //静音

		
		DAC_Current_Set(0);
		wave_init();					//电刺激初始化
		change_fre_out(10);		//10HZ
		wave_start();					//开启电刺激		
		

  while (1)
  {	

		KEY_Circle_Dete();		//按键循环检测
	
	
//    lockDMAToFrontPorch(true);		
    /* USER CODE END WHILE */

//  MX_TouchGFX_Process();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLRCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{//比较中断
// if(htim == &htim5)
// {
//	ASSR_ComINT_Handle();
// }
	wave_OC_Handle(htim);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	// system tick : 1ms
  if (htim->Instance == TIM7) 
	{
		res = f_typetell(play_name); 
		if(res == T_WAV)
		{
			res = wav_play_song(play_name);
			Audio_Process();
		}		
		touchgfx_60HZ_tick();		//touchgfx 心跳节拍 16ms
		blink_LED1();						//系统指示灯
		system_Key_tick();			//key tick
	//	Key_SetAudio();					//音量按键循环处理		
		Key_SetFEM();						//电刺激按键
  }
//  if (htim->Instance == TIM5) 
//	{
//		ASSR_EndINT_Handle();
//	}
}

//按键4的功能函数
void K4_fucntion()
{
	play_cnt++;
	if(play_cnt == 11)
		play_cnt = 1;
	switch(play_cnt)
	{
		case 1:play_name = Click_15HZ;	break;
		case 2:play_name = Click_30HZ;	break;	
		case 3:play_name = Click_35HZ;	break;	
		case 4:play_name = Click_40HZ;	break;	
		case 5:play_name = Click_45HZ;	break;	
		
		case 6:play_name = sine_40HZ;	break;	
		case 7:play_name = sine_45HZ;	break;	
		case 8:play_name = sine_50HZ;	break;	
		case 9:play_name = sine_55HZ;	break;	
		case 10:play_name = sine_60HZ;	break;			
	}
}
/* USER CODE END 4 */

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
		LED_3(1);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
