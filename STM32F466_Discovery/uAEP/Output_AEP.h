#ifndef _OUTPUT_AEP_H
#define _OUTPUT_AEP_H


#define 	click_mode		0x01
#define		comp_mode			0x02

#include "main.h"


void	ASSR_init(uint8_t mode);
void 	ASSR_ComINT_Handle();//比较中断
void	ASSR_EndINT_Handle();//一个周期结束
void	ASSR_state(uint8_t state);





















#endif






