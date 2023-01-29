#include "stm32f10x.h"  
#include "GPIO_config.h"
#include "usart.h"
#include "iic.h"
#include "mpu6050.h"
#include "systick_delay.h"
#include "stm32f10x_it.h"
#include <math.h>
#include <stdio.h>
#include "motor.h"
#include "hc_sr04.h"
#include "bmq.h"
#include "sys.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#define TASK_ENABLE 0

static float kp;
static float kd;

int main(void)
{
	double distance;
	float Pitch,Roll,Yaw;
	int X,Y;
	int i=0;
	kd=kp=0;
	SysTick_Init();//��ʼ��systick
	
	motor_Init();//�����ʼ��
	
	USART_Config();/* ����1ͨ�ų�ʼ�� */
	
	TIM2_PWM_Init(100,48);
	
    TIM1_PWM_Init(100,48);
	
	//Encoder_Init_TIM3();
	
	//Encoder_Init_TIM4();
	
	hc_sr04_Config();//��������ʼ��
	
	IIC_Init();
	MPU6050_initialize();     //=====MPU6050��ʼ��	
	DMP_Init();
	
	LED_Init();//ָʾ������mpu6050ok
	Delay_ms(3000);
	TOGGLE;
		while(1)
		{
			Read_DMP(&Pitch,&Roll,&Yaw);
			printf("Pitch: %f \nRoll: %f\nYaw: %f\n",Pitch,Roll,Yaw);
			if(i>=100)
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
			}
			Delay_ms(1000);
		}
}
//ָ���жϴ���
void DEBUG_USART1_IRQHandler(void)
{
	
	char i; 
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
		{
			i=USART_ReceiveData(DEBUG_USARTx);
			switch(i)
			{
				case 'a':kp+=0.1; break;
				case 'b':kd+=0.1; break;
				case 'c':kp-=0.1; break;
				case 'd':kd-=0.1; break;
				
			}
		}
}






