#ifndef _SYSTEM_ST7735_H
#define _SYSTEM_ST7735_H


#include "main.h"

#ifdef __cplusplus
extern   "C" {
#endif



#define RED  		0xf800
#define GREEN		0x07e0
#define BLUE 		0x001f
#define WHITE		0xffff
#define BLACK		0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D  
#define GRAY1   0x8410   
#define GRAY2   0x4208  
#define	LCD_W		128
#define LCD_H		160

void  SPI_WriteData(uint8_t Data);
void Lcd_WriteIndex(uint8_t Index);
void Lcd_WriteData(uint8_t Data);
void LCD_WriteData_16Bit(uint16_t Data);
void Lcd_WriteReg(uint8_t Index,uint8_t Data);
void Lcd_Reset(void);
void Lcd_Init(void);

void Lcd_Clear(uint16_t Color) ;


void touchgfx_60HZ_tick(void);

void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
#ifdef __cplusplus
        }
#endif


#endif