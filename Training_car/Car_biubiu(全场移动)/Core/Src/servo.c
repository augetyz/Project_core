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
    
    PWM1=500;
    PWM2=1500;
    PWM3=2500;
    PWM4=2500;
    PWM5=1800;
    PWM6=700;
}

servo_status servo_date_deal(uint8_t *date)
{
    static servo_status Servo={0,0,0,0,0,0,0,0};
    uint16_t value;
    if (date[0] != 0X55 || date[1] != 0X55)
    {
        Servo.sign=0;
        return Servo;
    }
    if (date[2] == 8)
    {
        value = date[8] + date[9] * 256;
        switch (date[7])
        {
        case 1:

            Servo.value_goal[0] = value;
            break;
        case 2:

            Servo.value_goal[1] = value;
            break;
        case 3:

            Servo.value_goal[2] = value;
            break;
        case 4:

            Servo.value_goal[3] = value;
            break;
        case 5:

            Servo.value_goal[4] = value;
            break;
        case 6:

            Servo.value_goal[5] = value;
            break;
        default:
            break;
        }
        Servo.sign=1;
        return Servo;
    }
    if (date[2] == 0X17)
    {
        if (date[7] == 0X01 && date[10] == 0X02 && date[13] == 0X03 && date[16] == 0X04 && date[19] == 0X05 && date[22] == 0X06)
        {
            Servo.time = date[5] + date[6] * 256;

            value = date[8] + date[9] * 256;
            
            Servo.value_goal[0] = value;

            value = date[11] + date[12] * 256;

            Servo.value_goal[1] = value;

            value = date[14] + date[15] * 256;

            Servo.value_goal[2] = value;

            value = date[17] + date[18] * 256;

            Servo.value_goal[3] = value;

            value = date[20] + date[21] * 256;

            Servo.value_goal[4] = value;

            value = date[23] + date[24] * 256;

            Servo.value_goal[5] = value;
        }
        Servo.sign=2;
        return Servo;
    }
    Servo.sign=0;
    return Servo;
}

