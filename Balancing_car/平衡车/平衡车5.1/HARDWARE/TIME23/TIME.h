#ifndef __TIME_H
#define __TIME_H	
#include "string.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "OLED.h"
#include "hc_sr04.h"
extern float Pitch,Roll,Yaw,distance;
extern int i;
void TIM4_Getsample_Int(u16 arr,u16 psc);

#endif
