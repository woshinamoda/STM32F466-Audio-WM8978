//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2018-10-31
//  ����޸�   : 
//  ��������   :��ʾ����(STM32F103ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   3.3v��Դ
//              SCL   PA5��SCLK��
//              SDA   PA7��MOSI��
//              RES   PD2
//              DC    PB5
//              BLK   PA4
//              MISO  PB6
//              CS1   PB7
//              CS2   PB8
//              PEN   PB9
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2018-10-31
//All rights reserved
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
#include "touch.h" 
int main(void)
{
	float t=0;
 	u16 lastpos[2];//���һ�ε�����
	delay_init();
	LED_Init();//LED��ʼ��
	LCD_Init();//LCD��ʼ��
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	lastpos[0]=0XFFFF;
	LED=0;

	LCD_ShowChinese(0,0,"�о�԰����",RED,WHITE,24,0);
	LCD_ShowString(24,30,"LCD_W:",RED,WHITE,16,0);
	LCD_ShowIntNum(72,30,LCD_W,3,RED,WHITE,16);
	LCD_ShowString(24,50,"LCD_H:",RED,WHITE,16,0);
	LCD_ShowIntNum(72,50,LCD_H,3,RED,WHITE,16);
	LCD_ShowFloatNum1(20,80,t,4,RED,WHITE,16);
	t+=0.11;
	LCD_ShowPicture(65,80,40,40,gImage_1);
	delay_ms(1000);
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
	TP_Init();
	LCD_ShowString(10,LCD_H-40,"X:",RED,WHITE,16,0);
	LCD_ShowIntNum(26,LCD_H-40,0,3,RED,WHITE,16);
	LCD_ShowString(10,LCD_H-20,"Y:",RED,WHITE,16,0);
	LCD_ShowIntNum(26,LCD_H-20,0,3,RED,WHITE,16);
	while(1)
	{
 		tp_dev.scan(0);//ɨ��
 		if(tp_dev.sta&TP_PRES_DOWN)//�а���������
		{				  
			delay_ms(1);//��Ҫ����ʱ,��������Ϊ�а�������.		    
			if((tp_dev.x[0]<(LCD_W-1)&&tp_dev.x[0]>=1)&&(tp_dev.y[0]<(LCD_H-1)&&tp_dev.y[0]>=1))
			{			 
				if(lastpos[0]==0XFFFF)
				{
					lastpos[0]=tp_dev.x[0];
					lastpos[1]=tp_dev.y[0];
				}
				LCD_DrawRoughLine(lastpos[0],lastpos[1],tp_dev.x[0],tp_dev.y[0],BLUE);//����
				lastpos[0]=tp_dev.x[0];
				lastpos[1]=tp_dev.y[0];
				LCD_ShowString(10,LCD_H-40,"X:",RED,WHITE,16,0);
				LCD_ShowIntNum(26,LCD_H-40,tp_dev.x[0],3,RED,WHITE,16);
				LCD_ShowString(10,LCD_H-20,"Y:",RED,WHITE,16,0);
				LCD_ShowIntNum(26,LCD_H-20,tp_dev.y[0],3,RED,WHITE,16);
			}    
		}
	}
}

