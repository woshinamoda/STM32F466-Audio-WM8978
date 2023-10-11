#include "system_led.h"

uint16_t	LED1_Cnt,LED2_Cnt,LED3_Cnt,LED4_Cnt;

void open_led(uint8_t num)
{
	switch(num)
	{
		case 1:
			LED_1(1);	break;
		case 2:
			LED_2(1);	break;	
		case 3:
			LED_3(1);	break;	
		case 4:
			LED_4(1);	break;	
	}
}

void close_led(uint8_t num)
{
	switch(num)
	{
		case 1:
			LED_1(0);	break;
		case 2:
			LED_2(0);	break;	
		case 3:
			LED_3(0);	break;	
		case 4:
			LED_4(0);	break;	
	}
}


void blink_LED1()
{
	LED1_Cnt++;
	switch(LED1_Cnt)
	{
		case 100 :	open_led(1);			break;
		case 200 : 	close_led(1); 		break;
		case 1000 : LED1_Cnt = 0;			break;
	}
}










