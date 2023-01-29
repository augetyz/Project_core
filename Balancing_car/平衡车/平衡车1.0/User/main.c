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
	
  
	SysTick_Init();//��ʼ��systick
	
	motor_Init();//�����ʼ��
	
	USART_Config();/* ����1ͨ�ų�ʼ�� */
	
	LED_Init();//�źŵƼ��
	
	TIM2_PWM_Init(1000,1440);
	
    TIM1_PWM_Init(1000,1440);
	
	Encoder_Init_TIM3();
	
	Encoder_Init_TIM4();
	
	hc_sr04_Config();//��������ʼ��
	
	I2C_Bus_Init();//I2C��ʼ��
  
	MPU6050_Init();//MPU6050��ʼ��

	if (MPU6050ReadID() == 1)//���MPU6050
	{	
	
		while(1)
		{
			
				MPU6050ReadAcc(Accel);		
				MPU6050ReadGyro(Gyro);
				Angle_Calcu(Accel,Gyro);
				//printf("Pitch:%d   Roll:%d  \nAAC_X:%2d   AAC_Y:%2d   AAC_Z:%2d \nGYRO_X:%2d    GYRO_Y:%2d    GYRO_Z:%2d \n",\
						(int)Angle_X_Final,(int)Angle_Y_Final,aacx,aacy,aacz,gyrox,gyroy,gyroz);
				MPU6050_ReturnTemp(&Temp); 			
				//Task_Delay[1]=100;//����һ�����ݣ��ɸ����Լ���������߲���Ƶ�ʣ���100ms����һ��
				X=(int)Angle_X_Final;Y=(int)Angle_Y_Final;
				printf("X: %d  Y: %d\n",X,Y);
				Delay_ms(100);
		}   
	}
	else
	{
			printf("\r\nû�м�⵽MPU6050��������\r\n");	
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






