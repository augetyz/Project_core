#include "soft_use.h"

#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include "doing.h"

BJ_status BJ_biubiu;

void delay(uint32_t time)
{
    uint32_t i, m;
    for (i = 0; i < time; i++)
    {
        for (m = 0; m < 36000; m++)
            ;
    }
}
void delay_us(uint32_t time)
{
    uint32_t i, m;
    for (i = 0; i < time; i++)
    {
        for (m = 0; m < 32; m++)
            ;
    }
}
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0)
        ;
    USART1->DR = (uint8_t)ch;
    return ch;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    case KEY1:
        if ((GPIOB->IDR & KEY1) == 0)
        {
            delay(10);
            if ((GPIOB->IDR & KEY1) == 0)
            {
                while ((GPIOB->IDR & KEY1) == 0)
                    ;
                // do something
                GPIOC->ODR ^= GPIO_PIN_14;
                printf("KEY1\n");
                BJ_motor_Ctrl(2, 2000);
            }
        }
        break;
    case KEY2:
        if ((GPIOA->IDR & KEY2) == 0)
        {
            delay(10);
            if ((GPIOA->IDR & KEY2) == 0)
            {
                while ((GPIOA->IDR & KEY2) == 0)
                    ;
                // do something
                GPIOC->ODR ^= GPIO_PIN_15;
                printf("KEY2\n");
                BJ_motor_Ctrl(2, -2000);
            }
        }
        break;
    case S_KEY1:
        break;
    case S_KEY2:
        break;
    case S_KEY3:
        break;
    case S_KEY4:
        break;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint32_t time = 0;
    if (htim->Instance == TIM1)
    {
        time++;
        if (time > 1000)
        {
            printf("biu\n");
            time = 0;
        }
    }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{

    static uint8_t BJ_step[4] = {1, 1, 1, 1};
    if (htim->Instance == TIM2)
    {
        if (BJ_biubiu.step_goal[0] != 0)
        {
            BJ_biubiu.step_use[0]++;
            if (BJ_step[0])
            {
                TIM2->CCR1 = 600;
                BJ_step[0] = 0;
            }
            if (BJ_biubiu.step_use[0] >= BJ_biubiu.step_goal[0])
            {
                BJ_biubiu.step_use[0] = BJ_biubiu.step_goal[0] = 0;
                TIM2->CCR1 = 0;
                BJ_step[0] = 1;
            }
        }

        if (BJ_biubiu.step_goal[1] != 0)
        {
            BJ_biubiu.step_use[1]++;
            if (BJ_step[1])
            {
                TIM2->CCR2 = 600;
                BJ_step[1] = 0;
            }
            if (BJ_biubiu.step_use[1] >= BJ_biubiu.step_goal[1])
            {
                BJ_biubiu.step_use[1] = BJ_biubiu.step_goal[1] = 0;
                TIM2->CCR2 = 0;
                BJ_step[1] = 1;
            }
        }

        if (BJ_biubiu.step_goal[2] != 0)
        {
            BJ_biubiu.step_use[2]++;
            if (BJ_step[2])
            {
                TIM2->CCR3 = 600;
                BJ_step[2] = 0;
            }
            if (BJ_biubiu.step_use[2] >= BJ_biubiu.step_goal[2])
            {
                BJ_biubiu.step_use[2] = BJ_biubiu.step_goal[2] = 0;
                TIM2->CCR3 = 0;
                BJ_step[2] = 1;
            }
        }

        if (BJ_biubiu.step_goal[3] != 0)
        {
            BJ_biubiu.step_use[3]++;
            if (BJ_step[3])
            {
                TIM2->CCR4 = 600;
                BJ_step[3] = 0;
            }
            if (BJ_biubiu.step_use[3] >= BJ_biubiu.step_goal[3])
            {
                BJ_biubiu.step_use[3] = BJ_biubiu.step_goal[3] = 0;
                TIM2->CCR4 = 0;
                BJ_step[3] = 1;
            }
        }
    }
}
