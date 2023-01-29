#ifndef _GPIO_CONFIG_H
#define _GPIO_CONFIG_H

#include "stm32f10x.h"
#define digitalToggle(p,i) {p->ODR ^=i;} 
#define TOGGLE	 digitalToggle(GPIOC,GPIO_Pin_13)

void LED_Init(void);
void motor_Init(void);
#endif 
