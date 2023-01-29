#ifndef __BSP_I2C_H
#define	__BSP_I2C_H

#include "stm32f10x.h"
#include "bsp_usart.h"


/**************************I2C参数定义，I2C1或I2C2********************************/
#define             SENSORS_I2Cx                                I2C2
#define             SENSORS_I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             SENSORS_I2C_CLK                             RCC_APB1Periph_I2C2
#define             SENSORS_I2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             SENSORS_I2C_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define             SENSORS_I2C_SCL_PORT                        GPIOB   
#define             SENSORS_I2C_SCL_PIN                         GPIO_Pin_10
#define             SENSORS_I2C_SDA_PORT                        GPIOB 
#define             SENSORS_I2C_SDA_PIN                         GPIO_Pin_11

/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/*信息输出*/
#define MPU_DEBUG_ON         1

#define MPU_INFO(fmt,arg...)           printf("<<-MPU-INFO->> "fmt"\n",##arg)
#define MPU_ERROR(fmt,arg...)          printf("<<-MPU-ERROR->> "fmt"\n",##arg)
#define MPU_DEBUG(fmt,arg...)          do{\
                                          if(MPU_DEBUG_ON)\
                                          printf("<<-MPU-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)


void I2C_Bus_Init(void);
uint8_t I2C_ByteWrite(u8 pBuffer, u8 WriteAddr);
uint8_t I2C_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
void I2C_WaitStandbyState(void);

#endif /* __BSP_I2C_H */


																					
//main()																					
/****************************************************************************************/
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
				//printf("Pitch:%d   Roll:%d  \nAAC_X:%2d   AAC_Y:%2d   AAC_Z:%2d \nGYRO_X:%2d    GYRO_Y:%2d    GYRO_Z:%2d \n",\
						(int)Angle_X_Final,(int)Angle_Y_Final,aacx,aacy,aacz,gyrox,gyroy,gyroz);
				MPU6050_ReturnTemp(&Temp); 
				//printf("温度： %8.2f\n",Temp);
				
				Task_Delay[1]=1;//更新一次数据，可根据自己的需求，提高采样频率，如100ms采样一次
				if(i==0){X=(int)Angle_X_Final;Y=(int)Angle_Y_Final;}
				i++;
				if(i==1000){printf("X: %d  Y: %d\n",X,Y);i=0;}
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
/************************************************************************************************************************/

























