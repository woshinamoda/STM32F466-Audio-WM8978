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
@brief	��wm8978д�Ĵ���
@Param 1���Ĵ�����ַ
@Param 2��д��ֵ
@retval ��0��(HAL_OK)�ɹ�
					�������������
****************************************************************/
uint8_t WM8978_Write_Reg(uint8_t regAddr,uint16_t Value)
{ 
	HAL_StatusTypeDef res;
	uint8_t reg, val;
	
	wm8978_RegTBL[regAddr]=Value;	//����Ĵ���ֵ������
//	
	//WM8978����Ϊ9λ
	reg =(regAddr << 1)|((Value >> 8)&0x01);		//��һ���ֽ�Ϊ�Ĵ�����ַ + �������λ
	val = Value & 0xff;													//�ڶ����ֽ�Ϊ���ݵ�8λ
	res = HAL_I2C_Mem_Write(&hi2c1, 0x34, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 0xFF);
	return res;
} 

/***************************************************************
@brief	��wm8978���Ĵ���
@retval �����ؼĴ���ֵ
****************************************************************/
uint16_t WM8978_Read_Reg(uint8_t reg)
{  
	return wm8978_RegTBL[reg];	
} 


/***************************************************************
@brief	��wm8978�����λ
@retval ��0��(HAL_OK)�ɹ�
					�������������
****************************************************************/
uint8_t	WM8978_Reset(void)
{
	uint8_t	res = 0xff;
	res = WM8978_Write_Reg(0,0);
	return res;
}

/***************************************************************
@brief	������������������
@Param 1: voll:����������(0~63)
@Param 2������������(0~63) 
@retval ��NONE
****************************************************************/ 
void WM8978_SetPhoneVolume(uint8_t voll,uint8_t volr)
{
	voll&=0X3F;
	volr&=0X3F;															//�޶���Χ
	if(voll==0)voll|=1<<6;									//����Ϊ0ʱ,ֱ��mute
	if(volr==0)volr|=1<<6;									//����Ϊ0ʱ,ֱ��mute 
	WM8978_Write_Reg(52,voll);							//R52,������������������
	WM8978_Write_Reg(53,volr|(1<<8));				//R53,������������������,ͬ������(HPVU=1)
}

/***************************************************************
@brief	�����������������ȣ�����
@Param 1: voll:����������(0~63) 
@retval ��NONE
****************************************************************/ 
void WM8978_SetSPKVolume(uint8_t volx)
{ 
	volx&=0X3F;//�޶���Χ
	if(volx==0)volx|=1<<6;									//����Ϊ0ʱ,ֱ��mute 
 	WM8978_Write_Reg(54,volx);							//R54,������������������
	WM8978_Write_Reg(55,volx|(1<<8));				//R55,������������������,ͬ������(SPKVU=1)	
}
/***************************************************************
@brief	��MIC�������ã�������BOOST��20dB�� MIC-->ADC���벿�ֵ����棩
@Param 1: 0~63,��Ӧ-12dB~35.25dB,0.75dB/Step
@retval ��NONE
****************************************************************/
void WM8978_SetMicGain(uint8_t gain)
{
	gain&=0X3F;
	WM8978_Write_Reg(45,gain);						//R45,��ͨ��PGA���� 
	WM8978_Write_Reg(46,gain|1<<8);				//R46,��ͨ��PGA����
}
/***************************************************************
@brief	��ADC����(L2\R2 Line IN)��������
@Param 1: 0~7��0��ֹ��1~7��Ӧ-12dB~6dB��3dB/Step
@retval ��NONE
****************************************************************/
void WM8978_SetLineGain(uint8_t gain)
{
	uint16_t regval;
	gain&=0X07;
	regval=WM8978_Read_Reg(47);						//��ȡR47
	regval&=~(7<<4);											//���ԭ�������� 
 	WM8978_Write_Reg(47,regval|gain<<4);	//����R47
	regval=WM8978_Read_Reg(48);						//��ȡR48
	regval&=~(7<<4);											//���ԭ�������� 
 	WM8978_Write_Reg(48,regval|gain<<4);	//����R48
} 
/***************************************************************
@brief	��AUXR,AUXL(PWM��Ƶ����)��������(AUXR/L-->ADC���벿�ֵ�����)
@Param 1: 0~7,0��ʾͨ����ֹ,1~7,��Ӧ-12dB~6dB,3dB/Step
@retval ��NONE
****************************************************************/
void WM8978_SetAUXGain(uint8_t gain)
{
	uint16_t regval;
	gain&=0X07;
	regval=WM8978_Read_Reg(47);						//��ȡR47
	regval&=~(7<<0);											//���ԭ�������� 
 	WM8978_Write_Reg(47,regval|gain<<0);	//����R47
	regval=WM8978_Read_Reg(48);						//��ȡR48
	regval&=~(7<<0);											//���ԭ�������� 
 	WM8978_Write_Reg(48,regval|gain<<0);	//����R48
} 
/*******************************************************************************
@brief	����������
@Param 1: 0����������	/	1������
@retval ��NONE
*******************************************************************************/
void WM8978_SetMute(uint32_t Cmd)
{
	uint16_t usRegValue;
	if(Cmd == 1)  //����
	{
		//����(OUT1)
		usRegValue = WM8978_Read_Reg(52); 
		usRegValue |= (1 << 6);
		WM8978_Write_Reg(52, usRegValue);

		usRegValue = WM8978_Read_Reg(53); 
		usRegValue |= (1 << 6);
		WM8978_Write_Reg(53, usRegValue);

		//����(OUT2)
		usRegValue = WM8978_Read_Reg(54); 
		usRegValue |= (1 << 6);
		WM8978_Write_Reg(54, usRegValue);

		usRegValue = WM8978_Read_Reg(55); 
		usRegValue |= (1 << 6);
		WM8978_Write_Reg(55, usRegValue);
	}
	else   		//ȡ������
	{
		//����(OUT1)
		usRegValue = WM8978_Read_Reg(52); 
		usRegValue &= ~(1 << 6);
		WM8978_Write_Reg(52, usRegValue);

		usRegValue = WM8978_Read_Reg(53); 
		usRegValue &= ~(1 << 6);
		WM8978_Write_Reg(53, usRegValue);

		//����(OUT2)
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
* Description    : WM8978���ù���ģʽ
* Input          : standard: 0,LSB(�Ҷ���);1,MSB(�����);2,�����ֱ�׼I2S;3,PCM/DSP
				   len:0,16λ;1,20λ;2,24λ;3,32λ
* Output         : None
* Return         : 0 �ɹ�  ���� ʧ��
* Note			 : None
*******************************************************************************/
void WM8978_Audio_FormatCFG(uint8_t standard, uint8_t len)
{
	//�޶���Χ
	standard &= 0x03;
	len &= 0x03;
	WM8978_Write_Reg(4,(standard << 3)|(len << 5));	//R4,WM8978����ģʽ����	

}
/*******************************************************************************
* Function Name  : WM8978_DAC_Set
* Description    : WM8978 DAC����
* Input          : 1 ʹ��DAC�����  0  ��ֹDAC�����
* Output         : None
* Return         : 0 �ɹ�  ���� ʧ��
* Note			 : None
*******************************************************************************/
void WM8978_DAC_Set(uint8_t dacen)
{
	uint16_t regval;
	
	regval = WM8978_Read_Reg(3);		//��ȡR3
	if(dacen)
	{
		regval |= 3 << 0;							//����DACR  DACL
	}
	else regval &= ~(3 << 0);				//�ر�DACR DACL
	
	 WM8978_Write_Reg(3,regval);		//����R3
	
	if(dacen) regval |= 1<<0;				//DAC���ʹ��

	 WM8978_Write_Reg(50,regval);   //R50����,Left DAC
	 WM8978_Write_Reg(51,regval);   //R51����,Right DAC
}
/*******************************************************************************
* Function Name  : WM8978_ADC_Set
* Description    : WM8978 ADC����
* Input          : 1 ʹ��ADC�����  0  ��ֹADC�����
* Output         : None
* Return         : 0 �ɹ�  ���� ʧ��
* Note			 : None
*******************************************************************************/
void WM8978_ADC_Set(uint8_t adcen, uint8_t micen, uint8_t line_en, uint8_t auxen)
{
	uint16_t regval,gain;
	
	regval = WM8978_Read_Reg(2);	//��ȡR2
	if(adcen)
	{
		regval |= 3 << 0;		//����ADCR  ADCL
	}
	else regval &= ~(3 << 0);		//�ر�ADCR ADCL
	
	 WM8978_Write_Reg(2,regval);	//����R2
	
	//����MIC
	regval = WM8978_Read_Reg(2);	//��ȡR2
	if(micen)regval |= 3<<2;		//����INPPGAENR,INPPGAENL(MIC��PGA�Ŵ�)
	else regval &= ~(3<<2);		//�ر�INPPGAENR,INPPGAENL.
 	 WM8978_Write_Reg(2,regval);	//����R2 
	
	//����LINE
	regval = WM8978_Read_Reg(44);	//��ȡR44
	if(line_en)
	{
		regval |= (3 << 4)|(3 << 0);	//����LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA
		gain = 5;
	}
	else 
	{
		regval &= ~(3 << 4|3 << 0);	//�ر�LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA
		gain = 0;
	}
	 WM8978_Write_Reg(44,regval);//����R44
	 WM8978_SetLineGain(gain);
	
	if(auxen)WM8978_SetAUXGain(7);		//AUX 6dB����
	else WM8978_SetAUXGain(0);			//�ر�AUX����  
}
/*******************************************************************************
* Function Name  : WM8978_SetOutputMode
* Description    : ���ģʽ����
* Input          : Output: ���ͨ��
				   OUTPUT_DEVICE_SPEAKER     �������
				   OUTPUT_DEVICE_HEADPHONE   �������
				   OUTPUT_DEVICE_BOTH        ����������
* Output         : None
* Return         : 0 �ɹ�  ���� ʧ��
* Note			 : None
*******************************************************************************/
void WM8978_SetOutputMode(uint8_t Output)
{
	uint16_t usRegValue1,usRegValue2;
	
	//��ȡR2 R3�Ĵ���
	usRegValue1 = WM8978_Read_Reg(2);
	usRegValue2 = WM8978_Read_Reg(3);

	switch (Output) 
	{
		case 0x01:    //�������
			//��ֹ�������
			usRegValue1 &= ~((1 << 8)|(1 << 7));
			WM8978_Write_Reg(2,usRegValue1);
			//ʹ���������
			usRegValue2 |= 0x6C;
			WM8978_Write_Reg(3,usRegValue2);
			break;

		case 0x02:   //�������
			//ʹ�ܶ������
			usRegValue1 |= 0x1B0;
			WM8978_Write_Reg(2,usRegValue1);
			//��ֹ�������
			usRegValue2 &= ~(0x6C);
			WM8978_Write_Reg(3,usRegValue2);
			break;
		
		case 0x03:
			//ʹ�ܶ������
			usRegValue1 |= 0x1B0;
			WM8978_Write_Reg(2,usRegValue1);
			//ʹ���������
			usRegValue2 |= 0x6C;
			WM8978_Write_Reg(3,usRegValue2);
			break;

		default: break;    
	}  
}
/*******************************************************************************
* Function Name  : WM8978_SetFrequency
* Description    : ��Ƶ����Ƶ������
* Input          : DeviceAddr: ������ַ
				   AudioFreq:  �����õ�Ƶ��
* Output         : None
* Return         : 0 �ɹ�  ���� ʧ��
* Note			 : WM8978֧��8K/12K/16K/24K/32K/48K
*******************************************************************************/
uint8_t WM8978_SetFrequency(uint32_t AudioFreq)
{
	uint8_t counter = 0;
	uint16_t usRegValue;

	usRegValue = WM8978_Read_Reg(7);  //��ȡ�Ĵ���R7��ֵ
	usRegValue &= ~(7 << 1);   //���bit1~bit3
	//����Ƶ��
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

		default:  //Ĭ��48K
			usRegValue |= (0 << 1); 
			counter += WM8978_Write_Reg(7, usRegValue);
		break; 
	}
	return counter;
}
/***************************************************************
@brief	����ʼ��wm8978
@retval ��HAL_OK
					��������ʼ��ʧ��
****************************************************************/ 
uint8_t WM8978_Init(uint8_t Volume)
{ 
	uint8_t res;
	res=WM8978_Write_Reg(0,0);						//��λWM8978
	if(res)return 1;											//����ָ��ʧ��,WM8978�쳣
	//����Ϊͨ������
	WM8978_Write_Reg(1,0X1B);							//R1,MICEN����Ϊ1(MICʹ��),BIASEN����Ϊ1(ģ��������),VMIDSEL[1:0]����Ϊ:11(5K)
	WM8978_Write_Reg(2,0X1B0);						//R2,ROUT1,LOUT1���ʹ��(�������Թ���),BOOSTENR,BOOSTENLʹ��
	WM8978_Write_Reg(3,0X6C);							//R3,LOUT2,ROUT2���ʹ��(���ȹ���),RMIX,LMIXʹ��	
	WM8978_Write_Reg(6,0);								//R6,MCLK���ⲿ�ṩ
	WM8978_Write_Reg(43,1<<4);						//R43,INVROUT2����,��������
	WM8978_Write_Reg(47,1<<8);						//R47����,PGABOOSTL,��ͨ��MIC���20������
	WM8978_Write_Reg(48,1<<8);						//R48����,PGABOOSTR,��ͨ��MIC���20������
	WM8978_Write_Reg(49,1<<1);						//R49,TSDEN,�������ȱ��� 
	WM8978_Write_Reg(49,1<<2);						//R49,SPEAKER BOOST,1.5x 
	WM8978_Write_Reg(10,1<<3);						//R10,SOFTMUTE�ر�,128x����,���SNR 
	WM8978_Write_Reg(14,1<<3);						//R14,ADC 128x������
		
	//����DAC
	WM8978_DAC_Set(1);			

	//����WM8978ΪI2S 16bit
	WM8978_Audio_FormatCFG(AUDIO_FORMAT_I2S, AUDIO_WORD_16BIT);
	
	//ȡ������
	WM8978_SetMute(0);
	
	//�������Ⱥ�����������
	WM8978_SetPhoneVolume(Volume,Volume);
	WM8978_SetSPKVolume(Volume);
	
} 

/***************************************************************
@brief	������3D������
@Param 1: depth:0~15(3Dǿ��,0����,15��ǿ)
@retval ��NONE
****************************************************************/ 
void WM8978_3D_Set(uint8_t depth)
{ 
	depth&=0XF;															//�޶���Χ 
 	WM8978_Write_Reg(41,depth);							//R41,3D�������� 	
}
/***************************************************************
@brief	������EQ/3D���÷���
@Param 1: 0,��ADC������
					1,��DAC������(Ĭ��)
@retval ��NONE
****************************************************************/ 
void WM8978_EQ_3D_Dir(uint8_t dir)
{
	uint16_t regval; 
	regval=WM8978_Read_Reg(0X12);
	if(dir)regval|=1<<8;
	else regval&=~(1<<8); 
 	WM8978_Write_Reg(18,regval);					//R18,EQ1�ĵ�9λ����EQ/3D����
}
/***************************************************************
@brief	������EQ1
@Param 1: cfreq:��ֹƵ��,0~3,�ֱ��Ӧ:80/105/135/175Hz
@Param 2������,0~24,��Ӧ-12~+12dB
@retval ��NONE
****************************************************************/ 
void WM8978_EQ1_Set(uint8_t cfreq,uint8_t gain)
{ 
	uint16_t regval;
	cfreq&=0X3;													//�޶���Χ 
	if(gain>24)gain=24;
	gain=24-gain;
	regval=WM8978_Read_Reg(18);
	regval&=0X100;
	regval|=cfreq<<5;										//���ý�ֹƵ�� 
	regval|=gain;												//��������	
 	WM8978_Write_Reg(18,regval);				//R18,EQ1���� 	
}
/***************************************************************
@brief	������EQ2
@Param 1: cfreq:��ֹƵ��,0~3,�ֱ��Ӧ:80/105/135/175Hz
@Param 2������,0~24,��Ӧ-12~+12dB
@retval ��NONE
****************************************************************/ 
void WM8978_EQ2_Set(uint8_t cfreq,uint8_t gain)
{ 
	uint16_t regval=0;
	cfreq&=0X3;													//�޶���Χ 
	if(gain>24)gain=24;
	gain=24-gain; 
	regval|=cfreq<<5;										//���ý�ֹƵ�� 
	regval|=gain;												//��������	
 	WM8978_Write_Reg(19,regval);				//R19,EQ2���� 	
}
/***************************************************************
@brief	������EQ3
@Param 1: cfreq:��ֹƵ��,0~3,�ֱ��Ӧ:80/105/135/175Hz
@Param 2������,0~24,��Ӧ-12~+12dB
@retval ��NONE
****************************************************************/ 
void WM8978_EQ3_Set(uint8_t cfreq,uint8_t gain)
{ 
	uint16_t regval=0;
	cfreq&=0X3;													//�޶���Χ 
	if(gain>24)gain=24;
	gain=24-gain; 
	regval|=cfreq<<5;										//���ý�ֹƵ�� 
	regval|=gain;												//��������	
 	WM8978_Write_Reg(20,regval);				//R20,EQ3���� 	
}
/***************************************************************
@brief	������EQ4
@Param 1: cfreq:��ֹƵ��,0~3,�ֱ��Ӧ:80/105/135/175Hz
@Param 2������,0~24,��Ӧ-12~+12dB
@retval ��NONE
****************************************************************/ 
void WM8978_EQ4_Set(uint8_t cfreq,uint8_t gain)
{ 
	uint16_t regval=0;
	cfreq&=0X3;													//�޶���Χ 
	if(gain>24)gain=24;
	gain=24-gain; 
	regval|=cfreq<<5;										//���ý�ֹƵ�� 
	regval|=gain;												//��������	
 	WM8978_Write_Reg(21,regval);				//R21,EQ4���� 	
}
/***************************************************************
@brief	������EQ5
@Param 1: cfreq:��ֹƵ��,0~3,�ֱ��Ӧ:80/105/135/175Hz
@Param 2������,0~24,��Ӧ-12~+12dB
@retval ��NONE
****************************************************************/ 
void WM8978_EQ5_Set(uint8_t cfreq,uint8_t gain)
{ 
	uint16_t regval=0;
	cfreq&=0X3;												//�޶���Χ 
	if(gain>24)gain=24;
	gain=24-gain; 
	regval|=cfreq<<5;									//���ý�ֹƵ�� 
	regval|=gain;											//��������	
 	WM8978_Write_Reg(22,regval);			//R22,EQ5���� 	
}


