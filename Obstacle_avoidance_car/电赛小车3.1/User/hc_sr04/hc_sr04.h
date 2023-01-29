#ifndef _HC_SR04_H
#define _HC_SR04_H
#include "stm32f10x.h"
#include "delay.h"

#include <stdio.h>

//超声波模块引脚定义
#define hc_sr04_GPIO GPIOC
#define hc_sr04_CLK RCC_APB2Periph_GPIOC
#define Trig GPIO_Pin_0
#define Echo GPIO_Pin_2

void Trig_biu(void);
void hc_sr04_Config(void);

float get_hc_sr(void);
#endif 
