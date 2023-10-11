#include "audio_wm8978_I2C.h"

static uint16_t wm8978_RegTBL[58] = {
	0x000, 0x000, 0x000, 0x000, 0x050, 0x000, 0x140, 0x000,
	0x000, 0x000, 0x000, 0x0FF, 0x0FF, 0x000, 0x100, 0x0FF,
	0x0FF, 0x000, 0x12C, 0x02C, 0x02C, 0x02C, 0x02C, 0x000,
	0x032, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x038, 0x00B, 0x032, 0x000, 0x008, 0x00C, 0x093, 0x0E9,
	0x000, 0x000, 0x000, 0x000, 0x003, 0x010, 0x010, 0x100,
	0x100, 0x002, 0x001, 0x001, 0x039, 0x039, 0x039, 0x039,
	0x001, 0x001
};

/***************************************************************
@brief	：wm8978写寄存器
@Param 1：寄存器地址
@Param 2：写入值
@retval ：0，(HAL_OK)成功
					其他，错误代码
****************************************************************/
uint8_t WM8978_Write_Reg(uint8_t regAddr,uint16_t Value)
{ 
	HAL_StatusTypeDef res;
	uint8_t reg, val;
	
	wm8978_RegTBL[regAddr]=Value;	//保存寄存器值到本地
//	
	//WM8978数据为9位
	reg =(regAddr << 1)|((Value >> 8)&0x01);		//第一个字节为寄存器地址 + 数据最高位
	val = Value & 0xff;													//第二个字节为数据低8位
	res = HAL_I2C_Mem_Write(&hi2c1, 0x34, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 0xFF);
	return res;
} 

/***************************************************************
@brief	：wm8978读寄存器
@retval ：返回寄存器值
****************************************************************/
uint16_t WM8978_Read_Reg(uint8_t reg)
{  
	return wm8978_RegTBL[reg];	
} 


/***************************************************************
@brief	：wm8978软件复位
@retval ：0，(HAL_OK)成功
					其他，错误代码
****************************************************************/
uint8_t	WM8978_Reset(void)
{
	uint8_t	res = 0xff;
	res = WM8978_Write_Reg(0,0);
	return res;
}

/***************************************************************
@brief	：设置左右声道音量
@Param 1: voll:左声道音量(0~63)
@Param 2：右声道音量(0~63) 
@retval ：NONE
****************************************************************/ 
void WM8978_SetPhoneVolume(uint8_t voll,uint8_t volr)
{
	voll&=0X3F;
	volr&=0X3F;															//限定范围
	if(voll==0)voll|=1<<6;									//音量为0时,直接mute
	if(volr==0)volr|=1<<6;									//音量为0时,直接mute 
	WM8978_Write_Reg(52,voll);							//R52,耳机左声道音量设置
	WM8978_Write_Reg(53,volr|(1<<8));				//R53,耳机右声道音量设置,同步更新(HPVU=1)
}

/***************************************************************
@brief	：设置扬声器（喇叭）音量
@Param 1: voll:左声道音量(0~63) 
@retval ：NONE
****************************************************************/ 
void WM8978_SetSPKVolume(uint8_t volx)
{ 
	volx&=0X3F;//限定范围
	if(volx==0)volx|=1<<6;									//音量为0时,直接mute 
 	WM8978_Write_Reg(54,volx);							//R54,喇叭左声道音量设置
	WM8978_Write_Reg(55,volx|(1<<8));				//R55,喇叭右声道音量设置,同步更新(SPKVU=1)	
}
/***************************************************************
@brief	：MIC增益设置（不包括BOOST的20dB， MIC-->ADC输入部分的增益）
@Param 1: 0~63,对应-12dB~35.25dB,0.75dB/Step
@retval ：NONE
****************************************************************/
void WM8978_SetMicGain(uint8_t gain)
{
	gain&=0X3F;
	WM8978_Write_Reg(45,gain);						//R45,左通道PGA设置 
	WM8978_Write_Reg(46,gain|1<<8);				//R46,右通道PGA设置
}
/***************************************************************
@brief	：ADC输入(L2\R2 Line IN)增益设置
@Param 1: 0~7，0禁止，1~7对应-12dB~6dB，3dB/Step
@retval ：NONE
****************************************************************/
void WM8978_SetLineGain(uint8_t gain)
{
	uint16_t regval;
	gain&=0X07;
	regval=WM8978_Read_Reg(47);						//读取R47
	regval&=~(7<<4);											//清除原来的设置 
 	WM8978_Write_Reg(47,regval|gain<<4);	//设置R47
	regval=WM8978_Read_Reg(48);						//读取R48
	regval&=~(7<<4);											//清除原来的设置 
 	WM8978_Write_Reg(48,regval|gain<<4);	//设置R48
} 
/***************************************************************
@brief	：AUXR,AUXL(PWM音频部分)增益设置(AUXR/L-->ADC输入部分的增益)
@Param 1: 0~7,0表示通道禁止,1~7,对应-12dB~6dB,3dB/Step
@retval ：NONE
****************************************************************/
void WM8978_SetAUXGain(uint8_t gain)
{
	uint16_t regval;
	gain&=0X07;
	regval=WM8978_Read_Reg(47);						//读取R47
	regval&=~(7<<0);											//清除原来的设置 
 	WM8978_Write_Reg(47,regval|gain<<0);	//设置R47
	regval=WM8978_Read_Reg(48);						//读取R48
	regval&=~(7<<0);											//清除原来的设置 
 	WM8978_Write_Reg(48,regval|gain<<0);	//设置R48
} 
/*******************************************************************************
@brief	：静音控制
@Param 1: 0：开启音乐	/	1：静音
@retval ：NONE
*******************************************************************************/
void WM8978_SetMute(uint32_t Cmd)
{
	uint16_t usRegValue;
	if(Cmd == 1)  //静音
	{
		//耳机(OUT1)
		usRegValue = WM8978_Read_Reg(52); 
		usRegValue |= (1 << 6);
		WM8978_Write_Reg(52, usRegValue);

		usRegValue = WM8978_Read_Reg(53); 
		usRegValue |= (1 << 6);
		WM8978_Write_Reg(53, usRegValue);

		//喇叭(OUT2)
		usRegValue = WM8978_Read_Reg(54); 
		usRegValue |= (1 << 6);
		WM8978_Write_Reg(54, usRegValue);

		usRegValue = WM8978_Read_Reg(55); 
		usRegValue |= (1 << 6);
		WM8978_Write_Reg(55, usRegValue);
	}
	else   		//取消静音
	{
		//耳机(OUT1)
		usRegValue = WM8978_Read_Reg(52); 
		usRegValue &= ~(1 << 6);
		WM8978_Write_Reg(52, usRegValue);

		usRegValue = WM8978_Read_Reg(53); 
		usRegValue &= ~(1 << 6);
		WM8978_Write_Reg(53, usRegValue);

		//喇叭(OUT2)
		usRegValue = WM8978_Read_Reg(54); 
		usRegValue &= ~(1 << 6);
		WM8978_Write_Reg(54, usRegValue);

		usRegValue = WM8978_Read_Reg(55); 
		usRegValue &= ~(1 << 6);
		WM8978_Write_Reg(55, usRegValue);
	}
}

/*******************************************************************************
* Function Name  : WM8978_Audio_FormatCFG
* Description    : WM8978设置工作模式
* Input          : standard: 0,LSB(右对齐);1,MSB(左对齐);2,飞利浦标准I2S;3,PCM/DSP
				   len:0,16位;1,20位;2,24位;3,32位
* Output         : None
* Return         : 0 成功  其它 失败
* Note			 : None
*******************************************************************************/
void WM8978_Audio_FormatCFG(uint8_t standard, uint8_t len)
{
	//限定范围
	standard &= 0x03;
	len &= 0x03;
	WM8978_Write_Reg(4,(standard << 3)|(len << 5));	//R4,WM8978工作模式设置	

}
/*******************************************************************************
* Function Name  : WM8978_DAC_Set
* Description    : WM8978 DAC设置
* Input          : 1 使能DAC及输出  0  禁止DAC及输出
* Output         : None
* Return         : 0 成功  其它 失败
* Note			 : None
*******************************************************************************/
void WM8978_DAC_Set(uint8_t dacen)
{
	uint16_t regval;
	
	regval = WM8978_Read_Reg(3);		//读取R3
	if(dacen)
	{
		regval |= 3 << 0;							//开启DACR  DACL
	}
	else regval &= ~(3 << 0);				//关闭DACR DACL
	
	 WM8978_Write_Reg(3,regval);		//设置R3
	
	if(dacen) regval |= 1<<0;				//DAC输出使能

	 WM8978_Write_Reg(50,regval);   //R50设置,Left DAC
	 WM8978_Write_Reg(51,regval);   //R51设置,Right DAC
}
/*******************************************************************************
* Function Name  : WM8978_ADC_Set
* Description    : WM8978 ADC设置
* Input          : 1 使能ADC及输出  0  禁止ADC及输出
* Output         : None
* Return         : 0 成功  其它 失败
* Note			 : None
*******************************************************************************/
void WM8978_ADC_Set(uint8_t adcen, uint8_t micen, uint8_t line_en, uint8_t auxen)
{
	uint16_t regval,gain;
	
	regval = WM8978_Read_Reg(2);	//读取R2
	if(adcen)
	{
		regval |= 3 << 0;		//开启ADCR  ADCL
	}
	else regval &= ~(3 << 0);		//关闭ADCR ADCL
	
	 WM8978_Write_Reg(2,regval);	//设置R2
	
	//设置MIC
	regval = WM8978_Read_Reg(2);	//读取R2
	if(micen)regval |= 3<<2;		//开启INPPGAENR,INPPGAENL(MIC的PGA放大)
	else regval &= ~(3<<2);		//关闭INPPGAENR,INPPGAENL.
 	 WM8978_Write_Reg(2,regval);	//设置R2 
	
	//设置LINE
	regval = WM8978_Read_Reg(44);	//读取R44
	if(line_en)
	{
		regval |= (3 << 4)|(3 << 0);	//开启LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA
		gain = 5;
	}
	else 
	{
		regval &= ~(3 << 4|3 << 0);	//关闭LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA
		gain = 0;
	}
	 WM8978_Write_Reg(44,regval);//设置R44
	 WM8978_SetLineGain(gain);
	
	if(auxen)WM8978_SetAUXGain(7);		//AUX 6dB增益
	else WM8978_SetAUXGain(0);			//关闭AUX输入  
}
/*******************************************************************************
* Function Name  : WM8978_SetOutputMode
* Description    : 输出模式控制
* Input          : Output: 输出通道
				   OUTPUT_DEVICE_SPEAKER     喇叭输出
				   OUTPUT_DEVICE_HEADPHONE   耳机输出
				   OUTPUT_DEVICE_BOTH        耳机和喇叭
* Output         : None
* Return         : 0 成功  其它 失败
* Note			 : None
*******************************************************************************/
void WM8978_SetOutputMode(uint8_t Output)
{
	uint16_t usRegValue1,usRegValue2;
	
	//读取R2 R3寄存器
	usRegValue1 = WM8978_Read_Reg(2);
	usRegValue2 = WM8978_Read_Reg(3);

	switch (Output) 
	{
		case 0x01:    //喇叭输出
			//禁止耳机输出
			usRegValue1 &= ~((1 << 8)|(1 << 7));
			WM8978_Write_Reg(2,usRegValue1);
			//使能喇叭输出
			usRegValue2 |= 0x6C;
			WM8978_Write_Reg(3,usRegValue2);
			break;

		case 0x02:   //耳机输出
			//使能耳机输出
			usRegValue1 |= 0x1B0;
			WM8978_Write_Reg(2,usRegValue1);
			//禁止喇叭输出
			usRegValue2 &= ~(0x6C);
			WM8978_Write_Reg(3,usRegValue2);
			break;
		
		case 0x03:
			//使能耳机输出
			usRegValue1 |= 0x1B0;
			WM8978_Write_Reg(2,usRegValue1);
			//使能喇叭输出
			usRegValue2 |= 0x6C;
			WM8978_Write_Reg(3,usRegValue2);
			break;

		default: break;    
	}  
}
/*******************************************************************************
* Function Name  : WM8978_SetFrequency
* Description    : 音频采样频率设置
* Input          : DeviceAddr: 器件地址
				   AudioFreq:  待设置的频率
* Output         : None
* Return         : 0 成功  其它 失败
* Note			 : WM8978支持8K/12K/16K/24K/32K/48K
*******************************************************************************/
uint8_t WM8978_SetFrequency(uint32_t AudioFreq)
{
	uint8_t counter = 0;
	uint16_t usRegValue;

	usRegValue = WM8978_Read_Reg(7);  //读取寄存器R7的值
	usRegValue &= ~(7 << 1);   //清除bit1~bit3
	//设置频率
	switch (AudioFreq)
	{
		case  AUDIO_FREQUENCY_8K:
			usRegValue |= (5 << 1); 
			counter += WM8978_Write_Reg(7, usRegValue);
		break;
		
		case  AUDIO_FREQUENCY_12K:
			usRegValue |= (4 << 1); 
			counter += WM8978_Write_Reg(7, usRegValue);
		break;

		case  AUDIO_FREQUENCY_16K:
			usRegValue |= (3 << 1); 
			counter += WM8978_Write_Reg(7, usRegValue);
		break;

		case  AUDIO_FREQUENCY_24K:
			usRegValue |= (2 << 1); 
			counter += WM8978_Write_Reg(7, usRegValue);
		break;

		case  AUDIO_FREQUENCY_32K:
			usRegValue |= (1 << 1); 
			counter += WM8978_Write_Reg(7, usRegValue);
		break;

		case  AUDIO_FREQUENCY_48K:
			usRegValue |= (0 << 1); 
			counter += WM8978_Write_Reg(7, usRegValue);
		break;

		default:  //默认48K
			usRegValue |= (0 << 1); 
			counter += WM8978_Write_Reg(7, usRegValue);
		break; 
	}
	return counter;
}
/***************************************************************
@brief	：初始化wm8978
@retval ：HAL_OK
					其他，初始化失败
****************************************************************/ 
uint8_t WM8978_Init(uint8_t Volume)
{ 
	uint8_t res;
	res=WM8978_Write_Reg(0,0);						//软复位WM8978
	if(res)return 1;											//发送指令失败,WM8978异常
	//以下为通用设置
	WM8978_Write_Reg(1,0X1B);							//R1,MICEN设置为1(MIC使能),BIASEN设置为1(模拟器工作),VMIDSEL[1:0]设置为:11(5K)
	WM8978_Write_Reg(2,0X1B0);						//R2,ROUT1,LOUT1输出使能(耳机可以工作),BOOSTENR,BOOSTENL使能
	WM8978_Write_Reg(3,0X6C);							//R3,LOUT2,ROUT2输出使能(喇叭工作),RMIX,LMIX使能	
	WM8978_Write_Reg(6,0);								//R6,MCLK由外部提供
	WM8978_Write_Reg(43,1<<4);						//R43,INVROUT2反向,驱动喇叭
	WM8978_Write_Reg(47,1<<8);						//R47设置,PGABOOSTL,左通道MIC获得20倍增益
	WM8978_Write_Reg(48,1<<8);						//R48设置,PGABOOSTR,右通道MIC获得20倍增益
	WM8978_Write_Reg(49,1<<1);						//R49,TSDEN,开启过热保护 
	WM8978_Write_Reg(49,1<<2);						//R49,SPEAKER BOOST,1.5x 
	WM8978_Write_Reg(10,1<<3);						//R10,SOFTMUTE关闭,128x采样,最佳SNR 
	WM8978_Write_Reg(14,1<<3);						//R14,ADC 128x采样率
		
	//开启DAC
	WM8978_DAC_Set(1);			

	//设置WM8978为I2S 16bit
	WM8978_Audio_FormatCFG(AUDIO_FORMAT_I2S, AUDIO_WORD_16BIT);
	
	//取消静音
	WM8978_SetMute(0);
	
	//设置喇叭和扬声器音量
	WM8978_SetPhoneVolume(Volume,Volume);
	WM8978_SetSPKVolume(Volume);
	
} 

/***************************************************************
@brief	：设置3D环绕声
@Param 1: depth:0~15(3D强度,0最弱,15最强)
@retval ：NONE
****************************************************************/ 
void WM8978_3D_Set(uint8_t depth)
{ 
	depth&=0XF;															//限定范围 
 	WM8978_Write_Reg(41,depth);							//R41,3D环绕设置 	
}
/***************************************************************
@brief	：设置EQ/3D作用方向
@Param 1: 0,在ADC起作用
					1,在DAC起作用(默认)
@retval ：NONE
****************************************************************/ 
void WM8978_EQ_3D_Dir(uint8_t dir)
{
	uint16_t regval; 
	regval=WM8978_Read_Reg(0X12);
	if(dir)regval|=1<<8;
	else regval&=~(1<<8); 
 	WM8978_Write_Reg(18,regval);					//R18,EQ1的第9位控制EQ/3D方向
}
/***************************************************************
@brief	：设置EQ1
@Param 1: cfreq:截止频率,0~3,分别对应:80/105/135/175Hz
@Param 2：增益,0~24,对应-12~+12dB
@retval ：NONE
****************************************************************/ 
void WM8978_EQ1_Set(uint8_t cfreq,uint8_t gain)
{ 
	uint16_t regval;
	cfreq&=0X3;													//限定范围 
	if(gain>24)gain=24;
	gain=24-gain;
	regval=WM8978_Read_Reg(18);
	regval&=0X100;
	regval|=cfreq<<5;										//设置截止频率 
	regval|=gain;												//设置增益	
 	WM8978_Write_Reg(18,regval);				//R18,EQ1设置 	
}
/***************************************************************
@brief	：设置EQ2
@Param 1: cfreq:截止频率,0~3,分别对应:80/105/135/175Hz
@Param 2：增益,0~24,对应-12~+12dB
@retval ：NONE
****************************************************************/ 
void WM8978_EQ2_Set(uint8_t cfreq,uint8_t gain)
{ 
	uint16_t regval=0;
	cfreq&=0X3;													//限定范围 
	if(gain>24)gain=24;
	gain=24-gain; 
	regval|=cfreq<<5;										//设置截止频率 
	regval|=gain;												//设置增益	
 	WM8978_Write_Reg(19,regval);				//R19,EQ2设置 	
}
/***************************************************************
@brief	：设置EQ3
@Param 1: cfreq:截止频率,0~3,分别对应:80/105/135/175Hz
@Param 2：增益,0~24,对应-12~+12dB
@retval ：NONE
****************************************************************/ 
void WM8978_EQ3_Set(uint8_t cfreq,uint8_t gain)
{ 
	uint16_t regval=0;
	cfreq&=0X3;													//限定范围 
	if(gain>24)gain=24;
	gain=24-gain; 
	regval|=cfreq<<5;										//设置截止频率 
	regval|=gain;												//设置增益	
 	WM8978_Write_Reg(20,regval);				//R20,EQ3设置 	
}
/***************************************************************
@brief	：设置EQ4
@Param 1: cfreq:截止频率,0~3,分别对应:80/105/135/175Hz
@Param 2：增益,0~24,对应-12~+12dB
@retval ：NONE
****************************************************************/ 
void WM8978_EQ4_Set(uint8_t cfreq,uint8_t gain)
{ 
	uint16_t regval=0;
	cfreq&=0X3;													//限定范围 
	if(gain>24)gain=24;
	gain=24-gain; 
	regval|=cfreq<<5;										//设置截止频率 
	regval|=gain;												//设置增益	
 	WM8978_Write_Reg(21,regval);				//R21,EQ4设置 	
}
/***************************************************************
@brief	：设置EQ5
@Param 1: cfreq:截止频率,0~3,分别对应:80/105/135/175Hz
@Param 2：增益,0~24,对应-12~+12dB
@retval ：NONE
****************************************************************/ 
void WM8978_EQ5_Set(uint8_t cfreq,uint8_t gain)
{ 
	uint16_t regval=0;
	cfreq&=0X3;												//限定范围 
	if(gain>24)gain=24;
	gain=24-gain; 
	regval|=cfreq<<5;									//设置截止频率 
	regval|=gain;											//设置增益	
 	WM8978_Write_Reg(22,regval);			//R22,EQ5设置 	
}


