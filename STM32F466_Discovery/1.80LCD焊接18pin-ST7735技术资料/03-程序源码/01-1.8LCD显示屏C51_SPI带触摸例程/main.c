//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  3.3v电源
//              SCL  P00（SCLK）
//              SDA  P01（MOSI）
//              RES  P02
//              DC   P03
//              BLK  P04
//              MISO P05
//              CS1  P06
//              CS2  P07
//              PEN  P16
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#include "REG51.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "touch.h" 


int main(void)
{
	float t=0;
 	u16 lastpos[2];//最后一次的数据
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	lastpos[0]=0XFFFF;

	LCD_ShowChinese(0,0,"中景园电子",RED,WHITE,24,0);
	LCD_ShowString(24,30,"LCD_W:",RED,WHITE,16,0);
	LCD_ShowIntNum(72,30,LCD_W,3,RED,WHITE,16);
	LCD_ShowString(24,50,"LCD_H:",RED,WHITE,16,0);
	LCD_ShowIntNum(72,50,LCD_H,3,RED,WHITE,16);
	LCD_ShowFloatNum1(20,80,t,4,RED,WHITE,16);
	t+=0.11;
	LCD_ShowPicture(65,80,40,40,gImage_1);
	delay_ms(1000);
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);

  TP_Init();//触摸初始化
	LCD_ShowString(10,LCD_H-40,"X:",RED,WHITE,16,0);
	LCD_ShowIntNum(26,LCD_H-40,0,3,RED,WHITE,16);
	LCD_ShowString(10,LCD_H-20,"Y:",RED,WHITE,16,0);
	LCD_ShowIntNum(26,LCD_H-20,0,3,RED,WHITE,16);
	while(1)
	{
 		if(Penirq==0)//有按键被按下
		{				  
			delay_ms(1);//必要的延时,否则老认为有按键按下.
			if(Convert_Pos())	//得到坐标值
			{			 
				if(lastpos[0]==0XFFFF)
				{
					lastpos[0]=tp_pixlcd.x;
					lastpos[1]=tp_pixlcd.y;
				}
				LCD_DrawRoughLine(lastpos[0],lastpos[1],tp_pixlcd.x,tp_pixlcd.y,BLUE);//画线
				lastpos[0]=tp_pixlcd.x;
				lastpos[1]=tp_pixlcd.y;
				LCD_ShowString(10,LCD_H-40,"X:",RED,WHITE,16,0);
				LCD_ShowIntNum(26,LCD_H-40,tp_pixlcd.x,3,RED,WHITE,16);
				LCD_ShowString(10,LCD_H-20,"Y:",RED,WHITE,16,0);
				LCD_ShowIntNum(26,LCD_H-20,tp_pixlcd.y,3,RED,WHITE,16);
			}    
		}
	}
}



	