#include "servo.h"

extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim12;

void servo_ctrl(uint8_t servo,uint16_t value)
{
    value=(float)value*100/Value_Ctrl+25;
    
    if(servo&(1<<0)) TIM9->CCR1  = value;
    if(servo&(1<<1)) TIM9->CCR2  = value;
    if(servo&(1<<2)) TIM10->CCR1 = value;
    if(servo&(1<<3)) TIM11->CCR1 = value;
    if(servo&(1<<4)) TIM12->CCR1 = value;
    if(servo&(1<<5)) TIM12->CCR2 = value;
}

void servo_config(void)
{
    HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim10,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim11,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);   
    
    servo_ctrl(servo1|servo2|servo3|servo4|servo5|servo6,90);//初始角度设置，根据最终姿态修改。
}



