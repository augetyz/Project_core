#ifndef _GPIO_CONFIG_H
#define _GPIO_CONFIG_H

#include "stm32f10x.h"
#define digitalToggle(p,i) {p->ODR ^=i;} 
#define TOGGLE	 digitalToggle(GPIOC,GPIO_Pin_13)
#define TOGGLE1	 digitalToggle(GPIOC,GPIO_Pin_14)
void LED_Init1(void);
void motor_Init(void);
void ledbiu(void);
#endif 
