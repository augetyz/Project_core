#ifndef _MOTOR_H
#define _MOTOR_H

#include "pwm.h"
#include "stm32f10x.h" 
#include "sys.h"
void speed_control(int lunzi,int v);
void Motor_GPIO_Config(void);
#endif

