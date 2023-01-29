#ifndef _HC_SR04_H
#define _HC_SR04_H
#include "stm32f10x.h"
#include "systick_delay.h"

#include <stdio.h>

//������ģ�����Ŷ���
#define hc_sr04_GPIO GPIOA
#define hc_sr04_CLK RCC_APB2Periph_GPIOA
#define Trig GPIO_Pin_0
#define Echo GPIO_Pin_1

void Trig_biu(void);
void hc_sr04_Config(void);

float get_hc_sr(void);
#endif 
