#ifndef _SERVO_H_
#define _SERVO_H_

#include "stm32f4xx_hal.h"
#include "main.h"

#define Value_Ctrl 180  //����Ƕȿ��Ʒ�Χ��0~Value_Ctrl  ���ڿ��ƺ���servo_ctrl����������

#define servo1 0X01
#define servo2 0X02
#define servo3 0X04
#define servo4 0X08
#define servo5 0X10
#define servo6 0X20

void servo_ctrl(uint8_t servo,uint16_t value);
void servo_config(void);    

#endif // !_SERVO_H_
