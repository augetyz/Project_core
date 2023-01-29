#ifndef _MOTOR_H
#define _MOTOR_H

#include "pwm.h"
#include "stm32f10x.h" 
#include "sys.h"

#define Car_Left  0x11
#define Car_Right 0x22
#define Car_Left_GO  PBout(0)=1;PBout(1)=0
#define Car_Right_GO  PBout(4)=1;PBout(5)=0
#define Car_Left_Back  PBout(0)=0;PBout(1)=1
#define Car_Right_Back  PBout(4)=0;PBout(5)=1
#define Car_Left_Stop  PBout(0)=0;PBout(1)=0
#define Car_Right_Stop  PBout(4)=0;PBout(5)=0
void Motor_GPIO_Config(void);
void speed_control(int lunzi,int v);
#endif

