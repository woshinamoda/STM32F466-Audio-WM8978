#ifndef _SYSTEM_KEY_H
#define _SYSTEM_KEY_H


#include "main.h"

#ifdef __cplusplus
extern   "C" {
#endif

extern struct Buf_KEY_cmd key_state;
extern struct Buf_KEY_value key_data;


struct Buf_KEY_cmd
{
	uint8_t		key_fall_flag;		//����ȷ�����ɱ�־λ
	uint8_t		key_up_flag;			//����ȷ��̧���־λ
	uint16_t	key_hold_ms;			//�����������³����ߵ�ƽʱ��
	uint8_t		short_key_flag;		//�̰���ʶ
	uint8_t		long_key_flag;		//������ʶ
};



struct Buf_KEY_value
{
	uint8_t	botton_flag;		//������ʶλ
	uint8_t K1_Flag;				//1
	uint8_t	K2_Flag;				//2
	uint8_t K3_Flag;				//3
	uint8_t K4_Flag;				//4
	
	uint8_t	K2_Long_Flag;
	uint8_t	K3_Long_Flag;
};


void system_Key_tick();
void KEY_Circle_Dete();




#ifdef __cplusplus
        }
#endif


#endif