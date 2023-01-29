#include "stm32f10x.h"  
#include "pwm.h"
#include "motor.h"
#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "usart.h"
#include <stdio.h>
#include "pid.h"
#include "encoder.h"
#include "TiMbase.h" 
/*
PWM  
LEFT PA6  RIGHT   PA7

ENCODER  
LEFT   PA15  PB3
RIGHT  PB6  PB7

方向
LEFT  PB0  PB1
RIGHT PB4  PB5

舵机 PA1
*/
//串口：TX：PB10 RX：PB11

extern int biu;
extern u8 biubiu[3];
extern float kp,kd,ki;
extern int goal1, goal2;
int main()
{
	USART_Config();
	printf("你是猪");  
	
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
    Motor_Config();	
    sg_biu(100);
    Motor_GPIO_Config();
	speed_control(Car_Left,0);speed_control(Car_Right,0);
	BASIC_TIM_Init();
	while(1)
	{
		;
	}
}
