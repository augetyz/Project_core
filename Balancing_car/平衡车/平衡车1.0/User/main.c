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
#define TASK_ENABLE 0


int main(void)
{
	short Accel[3];
	short Gyro[3];
	float Temp;
	int X,Y;
	
  
	SysTick_Init();//初始化systick
	
	motor_Init();//电机初始化
	
	USART_Config();/* 串口1通信初始化 */
	
	LED_Init();//信号灯检测
	
	TIM2_PWM_Init(1000,1440);
	
    TIM1_PWM_Init(1000,1440);
	
	Encoder_Init_TIM3();
	
	Encoder_Init_TIM4();
	
	hc_sr04_Config();//超声波初始化
	
	I2C_Bus_Init();//I2C初始化
  
	MPU6050_Init();//MPU6050初始化

	if (MPU6050ReadID() == 1)//检测MPU6050
	{	
	
		while(1)
		{
			
				MPU6050ReadAcc(Accel);		
				MPU6050ReadGyro(Gyro);
				Angle_Calcu(Accel,Gyro);
				//printf("Pitch:%d   Roll:%d  \nAAC_X:%2d   AAC_Y:%2d   AAC_Z:%2d \nGYRO_X:%2d    GYRO_Y:%2d    GYRO_Z:%2d \n",\
						(int)Angle_X_Final,(int)Angle_Y_Final,aacx,aacy,aacz,gyrox,gyroy,gyroz);
				MPU6050_ReturnTemp(&Temp); 			
				//Task_Delay[1]=100;//更新一次数据，可根据自己的需求，提高采样频率，如100ms采样一次
				X=(int)Angle_X_Final;Y=(int)Angle_Y_Final;
				printf("X: %d  Y: %d\n",X,Y);
				Delay_ms(100);
		}   
	}
	else
	{
			printf("\r\n没有检测到MPU6050传感器！\r\n");	
	}
}
void DEBUG_USART1_IRQHandler(void)
{
	
	char i; double distance;int m;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
		{
			i=USART_ReceiveData(DEBUG_USARTx);
			switch(i)
			{
				case 'a': motorA(0);TOGGLE;break;
				case 'b': motorA(1);break;
				case 'c': motorA(-1);break;
				case 'd': motorB(0);break;
				case 'e': motorB(1);break;
				case 'f': motorB(-1);break;
				case 'g': speed_A(100);break;
				case 'h': speed_B(100);break;
				case 'i': speed_A(500);break;
				case 'j': speed_B(500);break;
				case '1': TOGGLE;break;
				case 's': TOGGLE;distance=get_hc_sr();printf("%f\n",distance);break;
				case 'm': for(m=0;m<5;m++)
				{
					Delay_ms(1000);
					printf("A:%d B:%d\n",Read_Encoder(3),Read_Encoder(4));
					TOGGLE;
				}
			}
		}
}






