#include "intrins.h"
#include "touch.h"
#include "lcd.h"
#include "lcd_init.h"
#include "stdio.h"
u16 vx,vy;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
u16 chx,chy;//默认像素点坐标为0时的AD起始值

struct tp_pix_  tp_pixad,tp_pixlcd;	 //当前触控坐标的AD值,前触控坐标的像素值   

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;

#else
u8 CMD_RDX=0X90;
u8 CMD_RDY=0XD0;
#endif


u8 tpstate(void)
{
	return 	Penirq;
}
//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPI写数据
{
	unsigned char count=0;
	LCD_SCK=0;
	for(count=0;count<8;count++)
	{
	num<<=1;
	LCD_SDI=CY;
	LCD_SCK=0; _nop_();_nop_();_nop_();                //上升沿有效
	LCD_SCK=1; _nop_();_nop_();_nop_();
}
}
//**********************************************************
u16 ReadFromCharFrom7843()             //SPI 读数据
{
	u8 count=0;
	u16 Num=0;
	for(count=0;count<12;count++)
	{
		Num<<=1;		
		LCD_SCK=1; _nop_();_nop_();_nop_();                //下降沿有效
		LCD_SCK=0; _nop_();_nop_();_nop_();
		if(TOUCH_DOUT)
		{
			Num++;
		}
	}
	return(Num);
}	
//从7846/7843/XPT2046/UH7843/UH7846读取adc值	  0x90=y   0xd0-x
u16 ADS_Read_AD(unsigned char CMD)          
{
	u16 l;
	TOUCH_CS=0;
	WriteCharTo7843(CMD);        //送控制字即用差分方式读X坐标 详细请见有关资料
	LCD_SCK=1; _nop_();_nop_();_nop_();_nop_();
	LCD_SCK=0; _nop_();_nop_();_nop_();_nop_();
	l=ReadFromCharFrom7843();
	TOUCH_CS=1;
	return l;
}		   
//读取一个坐标值
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
#define READ_TIMES 15 //读取次数
#define LOST_VAL 5	  //丢弃值
u16 ADS_Read_XY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//带滤波的坐标读取
//最小值不能少于100.
u8 Read_ADS(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	 									   
	if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}
//2次读取ADS7846,连续读取2次有效的AD值,且这两次的偏差不能超过
//50,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
#define ERR_RANGE 20 //误差范围 
u8 Read_ADS2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=Read_ADS(&x1,&y1);   
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-ERR_RANGE内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)>>1;
        *y=(y1+y2)>>1;		
        return 1;
    }else return 0;	  
} 
//精确读取一次坐标,校准的时候用的	   
u8 Read_TP_Once(void)
{
	u8 re=0;
	u16 x1,y1;
	while(re==0)
	{
		while(!Read_ADS2(&tp_pixad.x,&tp_pixad.y));

		while(!Read_ADS2(&x1,&y1));
		if(tp_pixad.x==x1&&tp_pixad.y==y1)
		{
			re=1; 
		}
	} 
	return re;
}
//////////////////////////////////////////////////
//与LCD部分有关的函数  
//画一个触摸点
//用来校准用的
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	LCD_DrawLine(x-12,y,x+13,y,color);//横线
	LCD_DrawLine(x,y-12,x,y+13,color);//竖线
	LCD_DrawPoint(x+1,y+1,color);
	LCD_DrawPoint(x-1,y+1,color);
	LCD_DrawPoint(x+1,y-1,color);
	LCD_DrawPoint(x-1,y-1,color);
	Draw_Circle(x,y,6,color);//画中心圈
}

//画一个大点(2*2的点)		   
//x,y:坐标
//color:颜色
void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{	    
	LCD_DrawPoint(x,y,color);//中心点 
	LCD_DrawPoint(x+1,y,color);
	LCD_DrawPoint(x,y+1,color);
	LCD_DrawPoint(x+1,y+1,color);	 	  	
}

/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRoughLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		TP_Draw_Big_Point(uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
//转换结果
//根据触摸屏的校准参数来决定转换后的结果,保存在X0,Y0中
u8 Convert_Pos(void)
{		 	 
	u8 l=0; 
	if(Read_ADS2(&tp_pixad.x,&tp_pixad.y))
	{
		l=1;
		tp_pixlcd.x=tp_pixad.x>chx?((u32)tp_pixad.x-(u32)chx)*1000/vx:((u32)chx-(u32)tp_pixad.x)*1000/vx;
		tp_pixlcd.y=tp_pixad.y>chy?((u32)tp_pixad.y-(u32)chy)*1000/vy:((u32)chy-(u32)tp_pixad.y)*1000/vy;
	}
	return l;
}	   
//触摸屏校准代码
//得到四个校准参数
#define tp_pianyi    20
#define tp_xiaozhun 1000   //校准精度
void TP_Adjust(void)
{	
	float vx1,vx2,vy1,vy2;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
	u16 chx1,chx2,chy1,chy2;//默认像素点坐标为0时的AD起始值				 
	struct tp_pixu32_ p[4];
	u8  cnt=0;	 
	cnt=0;  
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);//清屏 
	LCD_ShowString(5,40,"Please adjust!",RED,WHITE,16,0);//显示提示信息
	TP_Drow_Touch_Point(20,20,RED);//画点1 
	while(1)
	{
		if(Penirq==0)//按键按下了
		{
			if(Read_TP_Once())//得到单次按键值
			{  								   
				p[cnt].x=tp_pixad.x;
				p[cnt].y=tp_pixad.y;
				cnt++; 
			}			 
			switch(cnt)
			{			   
				case 1:
					while(!Penirq)  //等待松手
					{
					}
					TP_Drow_Touch_Point(20,20,WHITE);			//清除点1 
					TP_Drow_Touch_Point(LCD_W-21,20,RED);	//画点2
					break;
				case 2:
					while(!Penirq)  //等待松手
					{
					}
 					TP_Drow_Touch_Point(LCD_W-21,20,WHITE);//清除点2
					TP_Drow_Touch_Point(20,LCD_H-21,RED);	 //画点3
					break;
				case 3:
					while(!Penirq)  //等待松手
					{
					}
 					TP_Drow_Touch_Point(20,LCD_H-21,WHITE);			//清除点3
 					TP_Drow_Touch_Point(LCD_W-21,LCD_H-21,RED);	//画点4
					break;
				case 4:	 //全部四个点已经得到
					
				  while(!Penirq)  //等待松手
					{
					}
			   		vx1=p[1].x>p[0].x?(p[1].x-p[0].x+1)*1000/(LCD_W-tp_pianyi-tp_pianyi):(p[0].x-p[1].x-1)*1000/(LCD_W-tp_pianyi-tp_pianyi);
				 	chx1=p[1].x>p[0].x?p[0].x-(vx1*tp_pianyi)/1000:p[0].x+(vx1*tp_pianyi)/1000;
				   	vy1=p[2].y>p[0].y?(p[2].y-p[0].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi):(p[0].y-p[2].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi);
					chy1=p[2].y>p[0].y?p[0].y-(vy1*tp_pianyi)/1000:p[0].y+(vy1*tp_pianyi)/1000; 
					
					vx2=p[3].x>p[2].x?(p[3].x-p[2].x+1)*1000/(LCD_W-tp_pianyi-tp_pianyi):(p[2].x-p[3].x-1)*1000/(LCD_W-tp_pianyi-tp_pianyi);
					chx2=p[3].x>p[2].x?p[2].x-(vx2*tp_pianyi)/1000:p[2].x+(vx2*tp_pianyi)/1000;
				   	vy2=p[3].y>p[1].y?(p[3].y-p[1].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi):(p[1].y-p[3].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi);
					chy2=p[3].y>p[1].y?p[1].y-(vy2*tp_pianyi)/1000:p[1].y+(vy2*tp_pianyi)/1000; 


					if((vx1>vx2&&vx1>vx2+tp_xiaozhun)||(vx1<vx2&&vx1<vx2-tp_xiaozhun)||(vy1>vy2&&vy1>vy2+tp_xiaozhun)||(vy1<vy2&&vy1<vy2-tp_xiaozhun))
					{
						cnt=0;
						TP_Drow_Touch_Point(LCD_W-21,LCD_H-21,WHITE);	//清除点4
						TP_Drow_Touch_Point(20,20,RED);//画点1 
						LCD_ShowString(5,40,"Adjust Filed! ",RED,WHITE,16,0);//校正失败提示
						LCD_ShowString(5,60,"Please Adjust!",RED,WHITE,16,0);//校正失败提示
						continue;
					}
					vx=(vx1+vx2)/2;
					vy=(vy1+vy2)/2;
					chx=(chx1+chx2)/2;
					chy=(chy1+chy2)/2;

					TP_Drow_Touch_Point(LCD_W-21,LCD_H-21,WHITE);	//清除点4
					LCD_ShowString(5,40," Adjust OK!   ",RED,WHITE,16,0);//校正完成
					LCD_ShowString(5,60,"              ",RED,WHITE,16,0);//清除文字
					delay_ms(1000);
 					LCD_ShowString(5,40,"              ",RED,WHITE,16,0);//清除文字
					return;//校正完成				 
			}
		}
	} 
}

//触摸屏初始化  		    
u8 TP_Init(void)
{
	u8 fac[20];
	if(Adujust)
	{
		if(USE_HORIZONTAL==0)
		{//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数
			vx=26738;
			vy=22812;
			chx=3765;
			chy=3932;
		}else if(USE_HORIZONTAL==1)
		{//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数
			vx=26107;
			vy=22620;
			chx=328;
			chy=161;
		}else if(USE_HORIZONTAL==2)
		{//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数
			vx=22141;
			vy=26579;
			chx=3793;
			chy=247;
		}else
		{//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数
			vx=22454;
			vy=26465;
			chx=194;
			chy=3764;
		}
		return 0;
	}else
	{
		TP_Adjust();//屏幕校准  
		//校准参数显示在LCD上，如果默认参数不准，请手动校正后自行修改
		sprintf((char*)fac,"vx:%d",vx);
		LCD_ShowString(10,LCD_H-118,fac,RED,WHITE,16,0);
		sprintf((char*)fac,"vy:%d",vy);
		LCD_ShowString(10,LCD_H-102,fac,RED,WHITE,16,0);
		sprintf((char*)fac,"chx:%d",chx);
		LCD_ShowString(10,LCD_H-86,fac,RED,WHITE,16,0);
		sprintf((char*)fac,"chy:%d",chy);
		LCD_ShowString(10,LCD_H-70,fac,RED,WHITE,16,0);
		return 1;
	}
}

