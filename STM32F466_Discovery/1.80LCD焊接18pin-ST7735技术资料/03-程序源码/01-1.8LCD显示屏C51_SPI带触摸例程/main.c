//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  3.3v��Դ
//              SCL  P00��SCLK��
//              SDA  P01��MOSI��
//              RES  P02
//              DC   P03
//              BLK  P04
//              MISO P05
//              CS1  P06
//              CS2  P07
//              PEN  P16
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
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
 	u16 lastpos[2];//���һ�ε�����
	LCD_Init();//LCD��ʼ��
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	lastpos[0]=0XFFFF;

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

  TP_Init();//������ʼ��
	LCD_ShowString(10,LCD_H-40,"X:",RED,WHITE,16,0);
	LCD_ShowIntNum(26,LCD_H-40,0,3,RED,WHITE,16);
	LCD_ShowString(10,LCD_H-20,"Y:",RED,WHITE,16,0);
	LCD_ShowIntNum(26,LCD_H-20,0,3,RED,WHITE,16);
	while(1)
	{
 		if(Penirq==0)//�а���������
		{				  
			delay_ms(1);//��Ҫ����ʱ,��������Ϊ�а�������.
			if(Convert_Pos())	//�õ�����ֵ
			{			 
				if(lastpos[0]==0XFFFF)
				{
					lastpos[0]=tp_pixlcd.x;
					lastpos[1]=tp_pixlcd.y;
				}
				LCD_DrawRoughLine(lastpos[0],lastpos[1],tp_pixlcd.x,tp_pixlcd.y,BLUE);//����
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



	