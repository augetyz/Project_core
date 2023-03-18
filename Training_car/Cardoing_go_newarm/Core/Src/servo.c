#include "servo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "soft_user.h"
// 使用定时器参数
/*

TIM9、TIM10、TIM11、TIM12

PSC 167 or 83
CNT 19999

舵机占空比 500~2500

*/

extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef htim11;
extern TIM_HandleTypeDef htim12;

servo_status Servo;

void servo_ctrl(uint8_t servo, uint16_t value)
{
    value = (float)value * 100 / Value_Ctrl + 25;

    if (servo & (1 << 0))
        PWM1 = value;
    if (servo & (1 << 1))
        PWM2 = value;
    if (servo & (1 << 2))
        PWM3 = value;
    if (servo & (1 << 3))
        PWM4 = value;
    if (servo & (1 << 4))
        PWM5 = value;
    if (servo & (1 << 5))
        PWM6 = value;
}

void servo_config(void)
{
    HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim11, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
    
//    PWM1=00;
//    PWM2=00;
//    PWM3=00;
//    PWM4=00;
//    PWM5=00;
//    PWM6=00;
    
    PWM1=600;
    PWM2=2450;
    PWM3=1180;
    PWM4=2000;
    PWM5=1200;
    PWM6=1500;
//    PWM1=1500;
//    PWM2=1500;
//    PWM3=1500;
//    PWM4=1500;
//    PWM5=1500;
//    PWM6=1500;
}
void servo_level_move(int16_t num)
{
    extern QueueHandle_t servo_Queue;
    servo_status Servo={800,550,2200,1900,1900,1500,1000,1};;
    num=num>500?num:(num<-500?-500:num);
    Servo.value_goal[0]=PWM1;
    Servo.value_goal[1]=PWM2;
    Servo.value_goal[2]=PWM3;
    Servo.value_goal[3]=PWM4;
    Servo.value_goal[4]=PWM5;
    Servo.value_goal[5]=PWM6+num;
    Servo.time=100;
    xQueueSend(servo_Queue,&Servo,0);
}
void servo_vertical_move(servo_status Servo)
{
    extern QueueHandle_t servo_Queue;
    
    Servo.value_goal[5]=PWM6;

    xQueueSend(servo_Queue,&Servo,0);
}
void servo_all_move(servo_status Servo)
{
    extern QueueHandle_t servo_Queue;
    
    xQueueSend(servo_Queue,&Servo,0);
}

servo_status servo_motion[]=
{  // 1    2    3    4    5   6   T 
    {600, 2450,1180,2000,1200,1500,2000},
    {600, 2450,1800,1000,1800,800,2000},
    {600, 550,2000,1900,1900,1460,2000},
};





