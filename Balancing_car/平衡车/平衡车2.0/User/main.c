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
	int X,Y,VA,VB;
	
	
	SysTick_Init();//初始化systick
	
	motor_Init();//电机初始化
	
	USART_Config();/* 串口1通信初始化 */
	
	TIM2_PWM_Init(100,48);
	
    TIM1_PWM_Init(100,48);
	
	Encoder_Init_TIM3();
	
	Encoder_Init_TIM4();
	
	hc_sr04_Config();//超声波初始化
	
	I2C_Bus_Init();//I2C初始化
  
	MPU6050_Init();//MPU6050初始化

	while(MPU6050ReadID() != 1);//检测MPU6050
	LED_Init();//指示灯亮起，mpu6050ok
		while(1)
		{
				MPU6050ReadAcc(Accel);		
				MPU6050ReadGyro(Gyro);
				Angle_Calcu(Accel,Gyro);
				VA=Read_Encoder(3);VB=Read_Encoder(4);
			printf("Pitch:%4d   Roll:%4d  \nAAC_X:%4d   AAC_Y:%4d   AAC_Z:%4d \nGYRO_X:%4d    GYRO_Y:%4d    GYRO_Z:%4d \n ",\
						(int)Angle_X_Final,(int)Angle_Y_Final,aacx,aacy,aacz,gyrox,gyroy,gyroz); 			
				//X=(int)Angle_X_Final;Y=(int)Angle_Y_Final;
			//printf("A: %d       B: %d\n",VA,VB);
				Delay_ms(50);
		}
}
//指令中断处理
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
				case 'g': speed_A(10);break;
				case 'h': speed_B(10);break;
				case 'i': speed_A(50);break;
				case 'j': speed_B(50);break;
				case '1': TOGGLE;break;
				case 's': TOGGLE;distance=get_hc_sr();printf("%f\n",distance);break;
				case 'm': 
					Delay_ms(1000);
					printf("A:%d B:%d\n",Read_Encoder(3),Read_Encoder(4));
					TOGGLE;break;
				
				case 'x': speed_A(25);break;
				case 'y': speed_B(25);break;
			}
		}
}






