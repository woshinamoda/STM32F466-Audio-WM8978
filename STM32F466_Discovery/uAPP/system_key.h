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
	uint8_t		key_fall_flag;		//按键确定生成标志位
	uint8_t		key_up_flag;			//按键确定抬起标志位
	uint16_t	key_hold_ms;			//按键触发按下持续高电平时长
	uint8_t		short_key_flag;		//短按标识
	uint8_t		long_key_flag;		//长按标识
};



struct Buf_KEY_value
{
	uint8_t	botton_flag;		//触发标识位
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