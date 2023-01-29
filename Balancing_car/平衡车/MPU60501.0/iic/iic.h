#ifndef __BSP_I2C_H
#define	__BSP_I2C_H

#include "stm32f10x.h"
#include "bsp_usart.h"


/**************************I2C�������壬I2C1��I2C2********************************/
#define             SENSORS_I2Cx                                I2C2
#define             SENSORS_I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             SENSORS_I2C_CLK                             RCC_APB1Periph_I2C2
#define             SENSORS_I2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             SENSORS_I2C_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define             SENSORS_I2C_SCL_PORT                        GPIOB   
#define             SENSORS_I2C_SCL_PIN                         GPIO_Pin_10
#define             SENSORS_I2C_SDA_PORT                        GPIOB 
#define             SENSORS_I2C_SDA_PIN                         GPIO_Pin_11

/*�ȴ���ʱʱ��*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/*��Ϣ���*/
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
				//printf("Pitch:%d   Roll:%d  \nAAC_X:%2d   AAC_Y:%2d   AAC_Z:%2d \nGYRO_X:%2d    GYRO_Y:%2d    GYRO_Z:%2d \n",\
						(int)Angle_X_Final,(int)Angle_Y_Final,aacx,aacy,aacz,gyrox,gyroy,gyroz);
				MPU6050_ReturnTemp(&Temp); 
				//printf("�¶ȣ� %8.2f\n",Temp);
				
				Task_Delay[1]=1;//����һ�����ݣ��ɸ����Լ���������߲���Ƶ�ʣ���100ms����һ��
				if(i==0){X=(int)Angle_X_Final;Y=(int)Angle_Y_Final;}
				i++;
				if(i==1000){printf("X: %d  Y: %d\n",X,Y);i=0;}
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
/************************************************************************************************************************/

























