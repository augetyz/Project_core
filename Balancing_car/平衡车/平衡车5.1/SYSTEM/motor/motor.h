#ifndef _MOTOR_H
#define _MOTOR_H

#include "stm32f10x.h"
#include "GPIO_config.h"
#include "pwm.h"
void motorA(int g);
void motorB(int g);
void speed_A(int g);
void speed_B(int g);
#endif
