#ifndef _BMQ_H
#define _BMQ_H

#include "stm32f10x.h"
#define ENCODER_TIM_PERIOD 2000

void Encoder_Init_TIM4(void);
void Encoder_Init_TIM3(void);
int Read_Encoder(u8 TIMX);


#endif

