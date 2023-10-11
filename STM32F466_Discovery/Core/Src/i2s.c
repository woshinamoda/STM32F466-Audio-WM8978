/**
  ******************************************************************************
  * @file    i2s.c
  * @brief   This file provides code for the configuration
  *          of the I2S instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "i2s.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_tx;

/* I2S2 init function */
void MX_I2S2_Init(void)
{

  /* USER CODE BEGIN I2S2_Init 0 */

  /* USER CODE END I2S2_Init 0 */

  /* USER CODE BEGIN I2S2_Init 1 */

  /* USER CODE END I2S2_Init 1 */
  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_44K;
  hi2s2.Init.CPOL = I2S_CPOL_LOW;
  hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S2_Init 2 */


  /* USER CODE END I2S2_Init 2 */

}

void HAL_I2S_MspInit(I2S_HandleTypeDef* i2sHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(i2sHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1;
    PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
    PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLI2SP_DIV2;
    PeriphClkInitStruct.PLLI2S.PLLI2SM = 12;
    PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
    PeriphClkInitStruct.PLLI2S.PLLI2SQ = 2;
    PeriphClkInitStruct.PLLI2SDivQ = 1;
    PeriphClkInitStruct.I2sApb1ClockSelection = RCC_I2SAPB1CLKSOURCE_PLLI2S;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* I2S2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2S2 GPIO Configuration
    PC1     ------> I2S2_SD
    PB10     ------> I2S2_CK
    PB12     ------> I2S2_WS
    PC6     ------> I2S2_MCK
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* I2S2 DMA Init */
    /* SPI2_TX Init */
    hdma_spi2_tx.Instance = DMA1_Stream4;
    hdma_spi2_tx.Init.Channel = DMA_CHANNEL_0;
    hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_spi2_tx.Init.Mode = DMA_CIRCULAR;
    hdma_spi2_tx.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_spi2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(i2sHandle,hdmatx,hdma_spi2_tx);

  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }
}

void HAL_I2S_MspDeInit(I2S_HandleTypeDef* i2sHandle)
{

  if(i2sHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**I2S2 GPIO Configuration
    PC1     ------> I2S2_SD
    PB10     ------> I2S2_CK
    PB12     ------> I2S2_WS
    PC6     ------> I2S2_MCK
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_1|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_12);

    /* I2S2 DMA DeInit */
    HAL_DMA_DeInit(i2sHandle->hdmatx);
  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#define DMA_MAX_SZE                         0xFFFF
#define AUDIODATA_SIZE          				    2   //* 16-bits audio data size */
#define DMA_MAX(x)           (((x) <= DMA_MAX_SZE)? (x):DMA_MAX_SZE)



//播放音频	pBuffer 缓存数据		Size 数据长度
uint8_t I2S_AUDIO_Play(uint16_t* pBuffer, uint32_t Size)
{
	HAL_I2S_Transmit_DMA(&hi2s2, pBuffer, DMA_MAX(Size/AUDIODATA_SIZE));
	//使能I2S2
	__HAL_I2S_ENABLE(&hi2s2);
	return 0;
}


//切换缓存
void I2S_AUDIO_ChangeBuffer(uint16_t *pData, uint16_t Size)
{
	HAL_I2S_Transmit_DMA(&hi2s2, pData, Size); 
}


//采样率表格                  8KHz  11KHz  16KHz  22KHz  32KHz  44KHz  48KHz  96KHz
const uint32_t I2SFreq[8] = {8000, 11025, 16000, 22050, 32000, 44100, 48000, 96000};
const uint32_t I2SPLLN[8] = {256, 429, 213, 429, 426, 271, 258, 344};
const uint32_t I2SPLLR[8] = {5, 4, 4, 4, 4, 4, 3, 1}; 

void I2S_AUDIO_SetFrequency(uint32_t AudioFreq)
{
	RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;
	uint8_t index = 0, freqindex = 0xFF;

	for(index = 0; index < (sizeof(I2SFreq)); index++)
	{
		if(I2SFreq[index] == AudioFreq)   //查找到支持的播放频率
		{
			freqindex = index;
		}
	}
	
	HAL_RCCEx_GetPeriphCLKConfig(&RCC_ExCLKInitStruct);
	if(freqindex != 0xFF)   //频率表中找到相应频率
	{
		/* I2S clock config 
		PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) ?(PLLI2SN/PLLM)
		I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
		RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1;
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SN = I2SPLLN[freqindex];
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SR = I2SPLLR[freqindex];
		HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct);     
	} 
	else //未找到相应频率,采用默认频率48KHz
	{
		/* I2S clock config 
		PLLI2S_VCO = f(VCO clock) = f(PLLI2S clock input) ?(PLLI2SN/PLLM)
		I2SCLK = f(PLLI2S clock output) = f(VCO clock) / PLLI2SR */
		RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S_APB1;
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SN = 258;
		RCC_ExCLKInitStruct.PLLI2S.PLLI2SR = 3;
		HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct); 
	}
	
	//初始化音频
	hi2s2.Init.AudioFreq = AudioFreq;				//IIS频率设置
	HAL_I2S_Init(&hi2s2);
}



void I2S2_ReInit(uint32_t I2S_Standard,uint32_t I2S_Mode,uint32_t I2S_Clock_Polarity,uint32_t I2S_DataFormat)
{
  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_Mode;
  hi2s2.Init.Standard = I2S_Standard;
  hi2s2.Init.DataFormat = I2S_DataFormat;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_DEFAULT;
  hi2s2.Init.CPOL = I2S_Clock_Polarity;
  hi2s2.Init.ClockSource = I2S_Clock_Polarity;
  hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    Error_Handler();
  }
}




//DMA传输完成回调,可外部重新定义
__weak void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
	
}

//DMA传输一半回调,可外部重新定义
__weak void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{ 
	
}

//DMA传输错误回调,可外部重新定义
__weak void BSP_AUDIO_OUT_Error_CallBack(void)
{
	
}


//I2S -WEAK 重定义： 传输完成回调
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	BSP_AUDIO_OUT_TransferComplete_CallBack();       
}

//I2S -WEAK 重定义： 传输一半回调
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	BSP_AUDIO_OUT_HalfTransfer_CallBack();   
}

//I2S 音频数据传输错误回调
void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s) 
{
	BSP_AUDIO_OUT_Error_CallBack();
}




/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
