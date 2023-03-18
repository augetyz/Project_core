#include "servo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
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
}

uint8_t servo_date_deal(uint8_t *date)
{
    uint16_t value;
    if (date[0] != 0X55 || date[1] != 0X55)
        return 0;
    if (date[2] == 8)
    {
        value = date[8] + date[9] * 256;
        switch (date[7])
        {
        case 1:
            Servo.value_last[0] = Servo.value_goal[0];
            Servo.value_goal[0] = value;
            break;
        case 2:
            Servo.value_last[1] = Servo.value_goal[1];
            Servo.value_goal[1] = value;
            break;
        case 3:
            Servo.value_last[2] = Servo.value_goal[2];
            Servo.value_goal[2] = value;
            break;
        case 4:
            Servo.value_last[3] = Servo.value_goal[3];
            Servo.value_goal[3] = value;
            break;
        case 5:
            Servo.value_last[4] = Servo.value_goal[4];
            Servo.value_goal[4] = value;
            break;
        case 6:
            Servo.value_last[5] = Servo.value_goal[5];
            Servo.value_goal[5] = value;
            break;
        default:
            break;
        }
        return 1;
    }
    if (date[2] == 0X17)
    {
        if (date[7] == 0X01 && date[10] == 0X02 && date[13] == 0X03 && date[16] == 0X04 && date[19] == 0X05 && date[22] == 0X06)
        {
            Servo.time = date[5] + date[6] * 256;

            value = date[8] + date[9] * 256;
            Servo.value_last[0] = Servo.value_goal[0];
            Servo.value_goal[0] = value;

            value = date[11] + date[12] * 256;
            Servo.value_last[1] = Servo.value_goal[1];
            Servo.value_goal[1] = value;

            value = date[14] + date[15] * 256;
            Servo.value_last[2] = Servo.value_goal[2];
            Servo.value_goal[2] = value;

            value = date[17] + date[18] * 256;
            Servo.value_last[3] = Servo.value_goal[3];
            Servo.value_goal[3] = value;

            value = date[20] + date[21] * 256;
            Servo.value_last[4] = Servo.value_goal[4];
            Servo.value_goal[4] = value;

            value = date[23] + date[24] * 256;
            Servo.value_last[5] = Servo.value_goal[5];
            Servo.value_goal[5] = value;
        }
        return 2;
    }
    return 3;
}
void servo_Ctrl(servo_status *servo)
{
    uint16_t time = 0, i = 0;
    uint16_t angle[6];
    if (servo->time == 0)
        time = 1000;
    else
        time = servo->time;
    time = time / 8;

    angle[0] = (servo->value_goal[0] - PWM1) / 8;
    angle[1] = (servo->value_goal[1] - PWM2) / 8;
    angle[2] = (servo->value_goal[2] - PWM3) / 8;
    angle[3] = (servo->value_goal[3] - PWM4) / 8;
    angle[4] = (servo->value_goal[4] - PWM5) / 8;
    angle[5] = (servo->value_goal[5] - PWM6) / 8;

    //    for(i=0;i<8;i++)
    //    {
    //        PWM1  += angle[0] ;
    //        PWM2  += angle[1] ;
    //        PWM3  += angle[2] ;
    //        PWM4  += angle[3] ;
    //        PWM5  += angle[4] ;
    //        PWM6  += angle[5] ;
    //        osDelay(10);
    //    }

    PWM1 = servo->value_goal[0];
    PWM2 = servo->value_goal[1];
    PWM3 = servo->value_goal[2];
    PWM4 = servo->value_goal[3];
    PWM5 = servo->value_goal[4];
    PWM6 = servo->value_goal[5];
}
