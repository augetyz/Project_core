#ifndef _BMQ_H
#define _BMQ_H

#include "stm32f10x.h"
#define ENCODER_TIM_PERIOD (u16)65535
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Get_Motor_Speed(int *leftSpeed,int *rightSpeed);
s16 getTIMx_DetaCnt(TIM_TypeDef * TIMx);

#endif

