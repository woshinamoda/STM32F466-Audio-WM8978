#ifndef SYSTEM_FEM_H
#define SYSTEM_FEM_H

#include "main.h"
#include "stdint.h"
#include "tim.h"


void wave_init();
void wave_start();
void wave_stop();
void change_fre_out(uint32_t fre);
void wave_OC_Handle(TIM_HandleTypeDef *htim);

void Key_SetFEM();











#endif

