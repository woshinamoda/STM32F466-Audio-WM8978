#ifndef _AUDIO_WM8978_I2C_H
#define _AUDIO_WM8978_I2C_H


#include "main.h"

#ifdef __cplusplus
extern   "C" {
#endif

#define WM8978_ADDR		0X1A	//WM8978的器件地址,固定为0X1A 

//WM8978支持的采样率
#define AUDIO_FREQUENCY_48K           ((uint32_t)48000)
#define AUDIO_FREQUENCY_32K           ((uint32_t)32000)
#define AUDIO_FREQUENCY_24K           ((uint32_t)24000)
#define AUDIO_FREQUENCY_16K           ((uint32_t)16000)
#define AUDIO_FREQUENCY_12K           ((uint32_t)12000) 
#define AUDIO_FREQUENCY_8K            ((uint32_t)8000) 

//WM8978模式
#define AUDIO_FORMAT_LSB        0
#define AUDIO_FORMAT_MSB        1
#define AUDIO_FORMAT_I2S      	2
#define AUDIO_FORMAT_PCM_DSP    3

//WM8978数据字长
#define AUDIO_WORD_16BIT		0
#define AUDIO_WORD_20BIT		1
#define AUDIO_WORD_24BIT		2
#define AUDIO_WORD_32BIT		3


	
#define EQ1_80Hz			0X00
#define EQ1_105Hz			0X01
#define EQ1_135Hz			0X02
#define EQ1_175Hz			0X03
//=========================
#define EQ2_230Hz			0X00
#define EQ2_300Hz			0X01
#define EQ2_385Hz			0X02
#define EQ2_500Hz			0X03
//=========================
#define EQ3_650Hz			0X00
#define EQ3_850Hz			0X01
#define EQ3_1100Hz		0X02
#define EQ3_14000Hz		0X03
//=========================
#define EQ4_1800Hz		0X00
#define EQ4_2400Hz		0X01
#define EQ4_3200Hz		0X02
#define EQ4_4100Hz		0X03
//=========================
#define EQ5_5300Hz		0X00
#define EQ5_6900Hz		0X01
#define EQ5_9000Hz		0X02
#define EQ5_11700Hz		0X03



uint8_t WM8978_Write_Reg(uint8_t regAddr,uint16_t Value);
uint16_t WM8978_Read_Reg(uint8_t reg);
uint8_t	WM8978_Reset(void);
void WM8978_SetPhoneVolume(uint8_t voll,uint8_t volr);
void WM8978_SetSPKVolume(uint8_t volx);
void WM8978_SetMicGain(uint8_t gain);
void WM8978_SetLineGain(uint8_t gain);
void WM8978_SetAUXGain(uint8_t gain);
void WM8978_SetMute(uint32_t Cmd);
void WM8978_Audio_FormatCFG(uint8_t standard, uint8_t len);
void WM8978_DAC_Set(uint8_t dacen);
void WM8978_ADC_Set(uint8_t adcen, uint8_t micen, uint8_t line_en, uint8_t auxen);
void WM8978_SetOutputMode(uint8_t Output);
uint8_t WM8978_SetFrequency(uint32_t AudioFreq);
uint8_t WM8978_Init(uint8_t Volume);


void WM8978_3D_Set(uint8_t depth);
void WM8978_EQ_3D_Dir(uint8_t dir);
void WM8978_EQ1_Set(uint8_t cfreq,uint8_t gain);
void WM8978_EQ2_Set(uint8_t cfreq,uint8_t gain);
void WM8978_EQ3_Set(uint8_t cfreq,uint8_t gain);
void WM8978_EQ4_Set(uint8_t cfreq,uint8_t gain);
void WM8978_EQ5_Set(uint8_t cfreq,uint8_t gain);

#ifdef __cplusplus
        }
#endif


#endif