#include "motor_ctrl.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern Car_status car_status;
void motor_tim_config(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); // 开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);     // 开启编码器定时器更新中断,防溢出处理
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); // 开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);     // 开启编码器定时器更新中断,防溢出处理
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL); // 开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);     // 开启编码器定时器更新中断,防溢出处理
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL); // 开启编码器定时器
    __HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);     // 开启编码器定时器更新中断,防溢出处理


    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}
void Motor_ctrl(uint8_t motor, uint16_t status)
{
     GPIOD->ODR |= (status << motor); // 硬件连线理想的话，一句话就可以控制四个电机任意四个状态
}

void speed_ctrl(uint8_t motor, int speed)
{
     Motor_ctrl(motor,
                speed > 0 ? Mode_gogo : (speed < 0 ? Mode_back : Mode_brake));
    switch (motor)
     {
     case Motor1:
          TIM1->CCR1 = speed > 0 ? speed : -speed;
          break;
     case Motor2:
          TIM1->CCR2 = speed > 0 ? speed : -speed;
          break;
     case Motor3:
          TIM1->CCR3 = speed > 0 ? speed : -speed;
          break;
     case Motor4:
          TIM1->CCR4 = speed > 0 ? speed : -speed;
          break;
     }
}
void speed_get(void)
{
     // 买的电机的编码器转一圈发出13440个脉冲。
        
      car_status.Car_speed[0] = TIM2->CNT - 6720;
      TIM2->CNT = 6720;

      car_status.Car_speed[1] = TIM3->CNT - 6720;
      TIM3->CNT = 6720;

      car_status.Car_speed[2] = TIM4->CNT - 6720;
      TIM4->CNT = 6720;

      car_status.Car_speed[3] = TIM5->CNT - 6720;
      TIM5->CNT = 6720;
     
}
