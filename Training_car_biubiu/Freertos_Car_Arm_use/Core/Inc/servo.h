#ifndef _SERVO_H_
#define _SERVO_H_

#include "stm32f4xx_hal.h"
#include "main.h"

#define Value_Ctrl 180  //舵机角度控制范围：0~Value_Ctrl  用于控制函数servo_ctrl的修整参数

#define servo1 0X01
#define servo2 0X02
#define servo3 0X04
#define servo4 0X08
#define servo5 0X10
#define servo6 0X20

#define PWM1 TIM10->CCR1
#define PWM2 TIM11->CCR1
#define PWM3 TIM12->CCR1
#define PWM4 TIM12->CCR2
#define PWM5 TIM9 ->CCR1
#define PWM6 TIM9 ->CCR2

typedef struct{
   uint16_t value_last[6];
   uint16_t value_goal[6];
   uint16_t time;
}servo_status;

void servo_date_deal(uint8_t *date);
void servo_ctrl(uint8_t servo,uint16_t value);
void servo_config(void);    
void servo_Ctrl(servo_status* servo);
#endif // !_SERVO_H_
