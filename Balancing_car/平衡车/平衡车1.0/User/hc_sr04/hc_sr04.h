#ifndef _HC_SR04_H
#define _HC_SR04_H
#include "stm32f10x.h"
#include "systick_delay.h"
#include "stm32f10x_tim.h"
#include <stdio.h>

//超声波模块引脚定义
#define hc_sr04_GPIO GPIOA
#define hc_sr04_CLK RCC_APB2Periph_GPIOA
#define Trig GPIO_Pin_6
#define Echo GPIO_Pin_7

void Trig_biu(void);
void hc_sr04_Config(void);
void trig_Init(void);
float get_hc_sr(void);
#endif 
