#ifndef _SYSTEM_LED_H
#define _SYSTEM_LED_H


#include "main.h"

#ifdef __cplusplus
extern   "C" {
#endif


void open_led(uint8_t num);
void close_led(uint8_t num);
void blink_LED1();




#ifdef __cplusplus
        }
#endif


#endif