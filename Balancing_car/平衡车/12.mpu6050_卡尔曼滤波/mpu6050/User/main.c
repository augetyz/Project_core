#include "stm32f10x.h"
#include "bsp_usart.h"
#include "iic.h"
#include "mpu6050.h"
#include "bsp_SysTick.h"
#include "stm32f10x_it.h"
#include <math.h>
#include "kalman.h"



#define TASK_ENABLE 0



extern unsigned int Task_Delay[NumOfTask];
/*
 * t : ��ʱʱ�� 
 * Ticks : ���ٸ�ʱ�����ڲ���һ���ж� 
 * f : ʱ��Ƶ�� 72000000
 * t = Ticks * 1/f = (72000000/100000) * (1/72000000) = 10us 
 */ 

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
  
int main(void)
{
	short Accel[3];
	short Gyro[3];
	float Temp;
	int i=0,X,Y;
  //��ʼ��systick
	SysTick_Init();
	
	/* ����1ͨ�ų�ʼ�� */
	USART1_Config();

	//I2C��ʼ��
	I2C_Bus_Init();
  //MPU6050��ʼ��
	MPU6050_Init();


	//���MPU6050
	if (MPU6050ReadID() == 1)
	{	
	
		while(1)
		{
			if(Task_Delay[0]==TASK_ENABLE)
			{
				Task_Delay[0]=1000;
			}
			
			if(Task_Delay[1]==0)
			{
				MPU6050ReadAcc(Accel);		
				MPU6050ReadGyro(Gyro);
				Angle_Calcu(Accel,Gyro);
				printf("AAC_Y:%.2f\n",Angle_Y_Final);
				
				//printf("�¶ȣ� %8.2f\n",Temp);
				
				Task_Delay[1]=10;//����һ�����ݣ��ɸ����Լ���������߲���Ƶ�ʣ���100ms����һ��
				
			}

			//*************************************	��������������ĸ�ʽ************************************//
	//		if(Task_Delay[i]==0)
	//		{
	//			Task(i);
	//			Task_Delay[i]=;
	//		}

		}   
	}
	else
	{
			printf("\r\nû�м�⵽MPU6050��������\r\n");	
	}
	
}





