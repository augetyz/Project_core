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
 * t : 定时时间 
 * Ticks : 多少个时钟周期产生一次中断 
 * f : 时钟频率 72000000
 * t = Ticks * 1/f = (72000000/100000) * (1/72000000) = 10us 
 */ 

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
  
int main(void)
{
	short Accel[3];
	short Gyro[3];
	float Temp;
	int i=0,X,Y;
  //初始化systick
	SysTick_Init();
	
	/* 串口1通信初始化 */
	USART1_Config();

	//I2C初始化
	I2C_Bus_Init();
  //MPU6050初始化
	MPU6050_Init();


	//检测MPU6050
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
				
				//printf("温度： %8.2f\n",Temp);
				
				Task_Delay[1]=10;//更新一次数据，可根据自己的需求，提高采样频率，如100ms采样一次
				
			}

			//*************************************	下面是增加任务的格式************************************//
	//		if(Task_Delay[i]==0)
	//		{
	//			Task(i);
	//			Task_Delay[i]=;
	//		}

		}   
	}
	else
	{
			printf("\r\n没有检测到MPU6050传感器！\r\n");	
	}
	
}





