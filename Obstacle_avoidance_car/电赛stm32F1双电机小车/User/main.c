#include "stm32f10x.h"  
#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "usart.h"
#include <stdio.h>
#include "TiMbase.h" 
#include "encoder.h"
#include "pwm.h"
#include "motor.h"


extern float   kp1,ki1,kd1;
extern float   kp2,ki2,kd2;
extern int speed1,speed2;
extern int goal1, goal2;


int main()
{
	USART_Config();
	delay_init();
	printf("你是猪\n\n");	
	Motor_Config();	
	Encoder_Init_TIM2();
    Encoder_Init_TIM4();
    TIM3_ETR();
	LED_Config();
	BASIC_TIM_Init();
	while(1)
	{
		
		delay_ms(400);
        printf("TIM4: %d\n",getTIMx_DetaCnt(TIM4));
		
		delay_ms(400);
        printf("TIM2: %d\n",getTIMx_DetaCnt(TIM2));
	}
}





/*********************************************************
两轮电机；
两个GPIO控制方向刹车，一个pwm控制速度，两个定时器通道测速

电机1：
GPIOE；Pin11,12控制正负转，TIM3，PA6，PWM输出，TIM2：PA15，PB3编码器测速

电机2：
GPIOE；Pin14,13控制正负转，TIM3，PA7，PWM输出，TIM4：PB7，PB6编码器测速


LED：

*********************************************************/



