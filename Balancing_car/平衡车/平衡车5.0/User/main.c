#include "stm32f10x.h"  
#include "GPIO_config.h"
#include "usart.h"
#include "iic.h"
#include "mpu6050.h"
#include "systick_delay.h"
#include "stm32f10x_it.h"
#include <math.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "motor.h"
#include "hc_sr04.h"
#include "bmq.h"
#include "oled.h"
#include "string.h"		
#include "sys.h"
#include "TIME.h"
static int kp=100;
static int kd;
static int X,Y=0;
float Pitch,Roll,Yaw;
int main(void)
{
//	u8 string[10] = {0};
	int i=0;
	
	SysTick_Init();//初始化systick
	
	motor_Init();//电机初始化
	
	USART_Config();/* 串口1通信初始化 */
	
    TIM1_PWM_Init(4800,0);
	
	TIM4_Getsample_Int(1999,7199);
	
	//Encoder_Init_TIM3();
	
	//Encoder_Init_TIM2();
	
	LED_Init();//指示灯亮起
	
	hc_sr04_Config();//超声波初始化
	
	/*OLED_Init();
	OLED_ShowString(40,0,(u8*)"MPU6050",12);
	OLED_Refresh();*/
	
	IIC_Init();
	MPU6050_initialize();     //=====MPU6050初始化	
	DMP_Init();

	
	for(i=0;i<6;i++)
	{
		TOGGLE;
		Delay_ms(500);
	}
		while(1)
		{
			Y=(int)Pitch-1;
			if(Y>0&&Y<90)
			{
					motorA(-1);motorB(-1);					
					speed_A(kp*Y+kd*X);speed_B(kp*Y+kd*X);					
			}
			if(Y<0&&Y>-90)
			{
				Y=-Y;
				motorA(1);motorB(1);	
				speed_A(kp*Y+kd*X);speed_B(kp*Y+kd*X);
			}
			/*if(Y>50||Y<-50)//歪成这样没救了
			{
				motorA(0);motorB(0);
			}*/
			
			/*if(i>=10)
			{
				i=0;
				distance=get_hc_sr();
				if((int)distance<=10)
				{
					motorA(0);motorB(0);
					GPIO_SetBits(GPIOC,GPIO_Pin_14);
				}
				else
					GPIO_ResetBits(GPIOC,GPIO_Pin_14);
			}*/
		}
}
//指令中断处理
void DEBUG_USART1_IRQHandler(void)
{
	char i; 
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
		{
			i=USART_ReceiveData(DEBUG_USARTx);
			switch(i)
			{
				case 'a':kp+=1; break;
				case 'b':kd+=1; break;
				case 'x':kp+=10; break;
				case 'y':kd+=10; break;
				case 'c':kp-=1; break;
				case 'd':kd-=1; break;
				case 'e':printf("kp:%d kd:%d\n",kp,kd);break;
				case 'f':printf("X:%d Y:%d\n",X,Y);break;
			}
		}
		
}
