#ifndef __TOUCH_H
#define __TOUCH_H	
#include "REG51.h"

#define u8  unsigned char
#define u16 unsigned int
#define u32 unsigned long int

#define Adujust      1        //1：使用出厂预设校准信息  0手动校准

//IO连接
sbit LCD_SCK	     =    P0^0;
sbit LCD_SDI       =    P0^1;	
sbit TOUCH_DOUT    =    P0^5;	
sbit TOUCH_CS      =    P0^7;																					   
sbit Penirq        =    P1^6;   //检测触摸屏响应信号

struct tp_pix_
{
	u16 x;
	u16 y;
};
struct tp_pixu32_
{
	u32 x;
	u32 y;
};
extern struct tp_pix_  tp_pixad,tp_pixlcd;	 //当前触控坐标的AD值,前触控坐标的像素值   
extern u16 vx,vy;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
extern u16 chx,chy;//默认像素点坐标为0时的AD起始值
u8 tpstate(void);
void WriteCharTo7843(unsigned char num);          //SPI写数据
u16 ReadFromCharFrom7843();             //SPI 读数据
u16 ADS_Read_AD(unsigned char CMD) ;
u16 ADS_Read_XY(u8 xy);
u8 Read_ADS(u16 *x,u16 *y);
u8 Read_ADS2(u16 *x,u16 *y) ;
u8 Read_TP_Once(void);
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color);
void TP_Draw_Big_Point(u16 x,u16 y,u16 color);
void LCD_DrawRoughLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
u8 Convert_Pos(void);
void TP_Adjust(void);
u8 TP_Init(void);
#endif  
	 
	 



