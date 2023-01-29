#include "stm32f10x.h"  
#include "GPIO_config.h"
#include "usart.h"
#include "iic.h"
#include "mpu6050.h"
#include "systick_delay.h"
#include "stm32f10x_it.h"
#include <math.h>
#include <stdio.h>
#include "kalman.h"
#include "motor.h"
#include "hc_sr04.h"
#include "bmq.h"

static int kp=100;//源哥哥说是60
static int kd=0;//说24
static int X,Y=0;
int mpu(void)
{
	int v,c;
	short Accel[3];
	short Gyro[3];
	for(v=0;v<50;v++)
	{
		MPU6050ReadAcc(Accel);		
		MPU6050ReadGyro(Gyro);
		Angle_Calcu(Accel,Gyro);
		c=(int)Angle_Y_Final;
	}
	return c;
}
int main(void)
{
	//double distance;
	short Accel[3];
	short Gyro[3];
	
	int i=0;
	SysTick_Init();//初始化systick
	
	motor_Init();//电机初始化
	
	USART_Config();/* 串口1通信初始化 */
	
	TIM2_PWM_Init(4800,0);
	
    TIM1_PWM_Init(4800,0);
	
	Encoder_Init_TIM3();
	
	Encoder_Init_TIM4();
	
	LED_Init();//指示灯亮起
	
	hc_sr04_Config();//超声波初始化
	
	I2C_Bus_Init();//I2C初始化
  
	MPU6050_Init();//MPU6050初始化

	while(MPU6050ReadID()!= 1);//检测MPU6050
	for(i=0;i<6;i++)
	{
		TOGGLE;
		Delay_ms(500);
	}
		while(1)
		{
			for(i=0;i<30;i++)
			{
				MPU6050ReadAcc(Accel);		
				MPU6050ReadGyro(Gyro);
				Angle_Calcu(Accel,Gyro);
			}
			Y=(int)Angle_Y_Final;
			X=Gyro[1]/16.384;
			if(X<0)
				X=-X;
			Y+=0.5;//机械中值
			i++;
			TOGGLE;
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
				case 'y':kp-=10; break;
				case 'c':kp-=1; break;
				case 'd':kd-=1; break;
				case 'e':printf("kp:%d kd:%d\n",kp,kd);break;
				case 'f':printf("X:%d Y:%d\n",X,Y);break;
			}
		}
		
}
