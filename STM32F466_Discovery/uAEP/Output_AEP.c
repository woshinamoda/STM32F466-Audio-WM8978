#include "output_aep.h"
#include "system_led.h"
#include "main.h"
#include "system_wavplay.h"  
/***********************************************************
	ASSR ʵ��̼���ʽ 
	32���ȼ���������̼� ���ȣ�819.2ms 		TIM5->����819.2ms  TIM5��Ԥװ�أ�819200 - 1(us)
											 ������409.6ms		TIM5->�Ƚ����ж�	 TIM5��	 CHR1: 409600(us)
																				TIM5->ASSR_cnt = 32pot
***********************************************************/

uint32_t	ASSR_cnt = 0; //1-32,����32�����ڣ�һ��̼�


void	ASSR_init(uint8_t mode)
{
	switch(mode)
	{
		case click_mode:
			break;
		case comp_mode:
			break;
	}
	TIM5->ARR = (1228800-1);
	TIM5->CCR1 = 409600;//409600;
	__HAL_TIM_DISABLE_OCxPRELOAD(&htim5, TIM_CHANNEL_1);
	TIM_CCxChannelCmd(TIM5,TIM_CHANNEL_1, TIM_CCx_ENABLE);
	HAL_TIM_OC_Start_IT(&htim5,TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&htim5);
}



void	ASSR_ComINT_Handle()//�Ƚ��ж�
{
	Audio_start();
	LED_2(1);		//trigger����
	Trigger(1);
}

void	ASSR_EndINT_Handle()//һ�����ڽ���
{
	ASSR_cnt++;
	if(ASSR_cnt == 1000)
	{
		ASSR_state(0);
	}
	else
	{
		Audio_stop();
		LED_2(0);	//trigger����
		Trigger(0);
	}
}


void	ASSR_state(uint8_t state)
{
	switch(state)
	{
		case 0: //�ر�ASSRģʽ
			ASSR_cnt = 0;
			HAL_TIM_OC_Stop_IT(&htim5,TIM_CHANNEL_1);
			HAL_TIM_Base_Stop_IT(&htim5);
			Audio_stop();
			LED_2(0);	
			Trigger(0);
			break;
		case 1: //����ASSR�̼�
			TIM5->CNT = 0;
			HAL_TIM_OC_Start_IT(&htim5,TIM_CHANNEL_1);
			HAL_TIM_Base_Start_IT(&htim5);
			break;
	}
}















































