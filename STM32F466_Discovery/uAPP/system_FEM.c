#include "system_fem.h"
#include "dac.h"


uint16_t Pulse_width=500;
uint8_t	 OC_cnt = 0;


void wave_init()
{
	//�ر�1/2/3�ıȽϼĴ�����Ԥװ�ڹ��ܣ��޸�CCR��������Ч
	__HAL_TIM_DISABLE_OCxPRELOAD(&htim2, TIM_CHANNEL_1);
	__HAL_TIM_DISABLE_OCxPRELOAD(&htim2,TIM_CHANNEL_2);
	//������ʱ��1/2/3ͨ���ıȽ��������
	TIM_CCxChannelCmd(TIM2,TIM_CHANNEL_1, TIM_CCx_ENABLE);
	TIM_CCxChannelCmd(TIM2,TIM_CHANNEL_2, TIM_CCx_ENABLE);


	TIM2->CCR1 = 700 - Pulse_width;
	TIM2->CCR2 = 800;
	//wave_stop();
}


void wave_start()
{
	__HAL_TIM_ENABLE(&htim2);	
	HAL_TIM_OC_Start_IT(&htim2,TIM_CHANNEL_1);			//��ͨ��1�Ƚ��ж�
	HAL_TIM_OC_Start_IT(&htim2,TIM_CHANNEL_2);			//��ͨ��2�Ƚ��ж�
}



void wave_stop()
{
	//__HAL_TIM_DISABLE(&htim2);
	HAL_TIM_OC_Stop_IT(&htim2,TIM_CHANNEL_1);				//��ͨ��1�Ƚ��ж�
	HAL_TIM_OC_Stop_IT(&htim2,TIM_CHANNEL_2);				//��ͨ��2�Ƚ��ж�
}



void change_fre_out(uint32_t fre)
{
	TIM2->ARR = (uint16_t)(1000000/fre)+1500;
}



void wave_OC_Handle(TIM_HandleTypeDef *htim)
{
	if(htim == &htim2)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{		
			if(OC_cnt == 0)
			{
				OC_cnt = 1;
				TIM2->CCR1 = 700;				
			}
			if(OC_cnt == 1)
			{
				OC_cnt = 2;
				TIM2->CCR2 = 800;
			}
	
		}
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if(OC_cnt == 2)
			{
				OC_cnt = 3;
				TIM2->CCR2 = 800 + Pulse_width;			
			}
			if(OC_cnt == 3)
			{
				OC_cnt = 0;
				TIM2->CCR1 = 700 - Pulse_width;
			}
		}
	}
}



uint8_t wave_out_dac=0;

#include "system_key.h"

void Key_SetFEM()
{
	if(key_data.K2_Flag == 1)
	{
		key_data.K2_Flag = 0;
//		wave_out_dac++;
//		if(wave_out_dac >= 100)
//			wave_out_dac = 100;
//		DAC_Current_Set(wave_out_dac);

		HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_SET);
		HAL_Delay(1);		
		HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_RESET);
		
	}

	if(key_data.K3_Flag == 1)
	{
		key_data.K3_Flag = 0;
//		if(wave_out_dac == 0)
//			wave_out_dac = 0;
//		else
//			wave_out_dac--;
//		DAC_Current_Set(wave_out_dac);

		HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_SET);
		HAL_Delay(1);		
		HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_RESET);

		
	}

	if(key_data.K4_Flag == 1)
	{
		key_data.K4_Flag = 0;		
		
		HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_RESET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_SET);
		HAL_Delay(1);		
		HAL_GPIO_WritePin(Trigger_GPIO_Port, Trigger_Pin, GPIO_PIN_RESET);		

	}


}




























