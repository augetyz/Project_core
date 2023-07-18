#include "doing.h"

#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "soft_use.h"
/********************************************************************************************************/ /*
 �������: | ���һ  |  �����  |  �����  |  ����� |  LED2  |  LED3  |  LED4  |
 step���ƣ�| PA0     |  PA1     |  PA2     |  PA3    |  PC13  |  PC14  |  PC15  |
 dir���� ��| PA4     |  PA5     |  PB12    |  PB13   |

 �������  ��| KEY1  | KEY2  |  ��λ����1  |  ��λ����2  |  ��λ����3  |  ��λ����4 |
 �͵�ƽ��Ч��| PB2   | PA8   |  PB3        |  PA11       |  PB4        |  PA12      |

 */
                                                                                                           /********************************************************************************************************/

extern BJ_status BJ_biubiu;

void BJ_motor_Ctrl(uint8_t Motor, int speed)
{
    switch (Motor)
    {
    case 1:
        if (speed < 0)
        {
            GPIOA->ODR |= GPIO_PIN_4;
            BJ_biubiu.step_goal[0] = -speed;
        }
        else
        {
            GPIOA->ODR &= ~GPIO_PIN_4;
            BJ_biubiu.step_goal[0] = speed;
        }
        BJ_biubiu.step_use[0] = 0;
        break;
    case 2:
        if (speed < 0)
        {
            GPIOA->ODR |= GPIO_PIN_5;
            BJ_biubiu.step_goal[1] = -speed;
        }
        else
        {
            GPIOA->ODR &= ~GPIO_PIN_5;
            BJ_biubiu.step_goal[1] = speed;
        }
        BJ_biubiu.step_use[1] = 0;
        break;

    case 3:
        if (speed < 0)
        {
            GPIOB->ODR |= GPIO_PIN_12;
            BJ_biubiu.step_goal[2] = -speed;
        }
        else
        {
            GPIOB->ODR &= ~GPIO_PIN_12;
            BJ_biubiu.step_goal[2] = speed;
        }
        BJ_biubiu.step_use[2] = 0;
        break;
    case 4:
        if (speed < 0)
        {
            GPIOB->ODR |= GPIO_PIN_13;
            BJ_biubiu.step_goal[3] = -speed;
        }
        else
        {
            GPIOB->ODR &= ~GPIO_PIN_13;
            BJ_biubiu.step_goal[3] = speed;
        }
        BJ_biubiu.step_use[3] = 0;
        break;
    }
}
