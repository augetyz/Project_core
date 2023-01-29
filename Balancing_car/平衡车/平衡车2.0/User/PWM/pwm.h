#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f10x.h"

#include "systick_delay.h"



#define RCC_APB1Periph_TIM2_Def        RCC_APB1Periph_TIM2
#define RCC_APB2Periph_GPIO_TIM2_Def   RCC_APB2Periph_GPIOB
#define GPIO_Pin_TIM2_Def              GPIO_Pin_3
#define GPIO_TIM2_Def                  GPIOB
#define TIM2_Def                       TIM2
#define TIM2_OC_CH_Init                TIM_OC2Init
#define TIM2_OC_CH_PreloadConfig       TIM_OC2PreloadConfig

#define RCC_APB1Periph_TIM3_Def        RCC_APB1Periph_TIM3
#define RCC_APB2Periph_GPIO_TIM3_Def   RCC_APB2Periph_GPIOB
#define GPIO_Pin_TIM3_Def              GPIO_Pin_5
#define GPIO_TIM3_Def                  GPIOB
#define TIM3_Def                       TIM3
#define TIM3_OC_CH_Init                TIM_OC2Init//ͨ��2
#define TIM3_OC_CH_PreloadConfig       TIM_OC2PreloadConfig


#define RCC_APB1Periph_TIM4_Def        RCC_APB1Periph_TIM4
#define RCC_APB2Periph_GPIO_TIM4_Def   RCC_APB2Periph_GPIOB
#define GPIO_Pin_TIM4_Def              GPIO_Pin_6
#define GPIO_TIM4_Def                  GPIOB
#define TIM4_Def                       TIM4
#define TIM4_OC_CH_Init                TIM_OC1Init//ͨ��1
#define TIM4_OC_CH_PreloadConfig       TIM_OC1PreloadConfig

void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr,u16 psc);



#endif 


