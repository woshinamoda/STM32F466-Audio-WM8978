#ifndef _OUTPUT_AEP_H
#define _OUTPUT_AEP_H


#define 	click_mode		0x01
#define		comp_mode			0x02

#include "main.h"


void	ASSR_init(uint8_t mode);
void 	ASSR_ComINT_Handle();//�Ƚ��ж�
void	ASSR_EndINT_Handle();//һ�����ڽ���
void	ASSR_state(uint8_t state);





















#endif






