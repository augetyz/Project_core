#ifndef __TIME_H
#define __TIME_H	
#include "string.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
 #include "GPIO_config.h"
extern float Pitch,Roll,Yaw;

void TIM4_Getsample_Int(u16 arr,u16 psc);

#endif
