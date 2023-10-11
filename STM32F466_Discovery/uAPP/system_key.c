#include "system_key.h"

struct Buf_KEY_cmd		key_state	={0};
struct Buf_KEY_value	key_data	={0};

void HAL_GPIO_EXTI_Callback(uint16_t	GPIO_Pin)
{
	if((KEY_1==1)||(KEY_2==1)||(KEY_3==1)||(KEY_4==1))
	{
		key_state.key_fall_flag = 1;	
	}

	if(KEY_1 == GPIO_PIN_SET)
		 key_data.botton_flag= 1;

	if(KEY_2 == GPIO_PIN_SET)
		 key_data.botton_flag= 2;
	
	if(KEY_3 == GPIO_PIN_SET)
		 key_data.botton_flag= 3;	

	if(KEY_4 == GPIO_PIN_SET)
		 key_data.botton_flag= 4;
	
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);	
}


void system_Key_tick()
{
	if(key_state.key_fall_flag==1)//�������������¼�
	{
		if((KEY_1==1)||(KEY_2==1)||(KEY_3==1)||(KEY_4==1))
		{   
				if(key_state.key_hold_ms <= 2000)  
				 {
						key_state.key_hold_ms ++; 
				 } 
				 else //�������µ�2000ms���жϳ���ʱ����������ɳ�����־
				 { 
					 key_state.key_hold_ms  = 0; 
					 key_state.short_key_flag=0;//��̰�����־
					 key_state.long_key_flag = 1;//��������־��λ
					 key_state.key_fall_flag = 0;//�尴�����±�־
				} 
		 } 
		else //����̧��
		{ 
			 if(key_state.key_hold_ms>50)//����ʱ�����50ms�����ɵ�����־
				{  
					key_state.key_hold_ms=0;
					key_state.short_key_flag=1;
					key_state.long_key_flag  =0;
					key_state.key_fall_flag=0;
					key_state.key_up_flag = 1;//����̧�𰴼������ɰ���̧���־

				} 
			 else  //��������ʱ��С��50ms,����
				{    
					key_state.key_hold_ms=0; 
					key_state.short_key_flag=0;
					key_state.long_key_flag =0;
					key_state.key_fall_flag=0;
				} 
	   	}
		}
}


void KEY_Circle_Dete()
{
	if(key_state.short_key_flag == 1)
	{
		key_state.short_key_flag = 0;
		if(key_data.botton_flag == 1)
		{
			key_data.K1_Flag = 1;
		}
		else if(key_data.botton_flag == 2)
		{
			key_data.K2_Flag = 1;
		}
		else if(key_data.botton_flag == 3)
		{
			key_data.K3_Flag = 1;
		}
		else if(key_data.botton_flag == 4)
		{
			key_data.K4_Flag = 1;
		}		
	}
	if(key_state.long_key_flag == 1)
	{
		key_state.long_key_flag = 0;
		if(key_data.botton_flag == 2)
		{
			key_data.K2_Long_Flag = 1;
		}
		if(key_data.botton_flag == 3)
		{
			key_data.K3_Long_Flag = 1;
		}	
	}		
}










