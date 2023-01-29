#include "stm32f10x.h"
#include "sys.h"
//////////////////////////全局变量的定义////////////////////////////////////  
float pitch,roll,yaw; 								  			 //欧拉角(姿态角)
short aacx,aacy,aacz;													 //加速度传感器原始数据
short gyrox,gyroy,gyroz;											 //陀螺仪原始数据
float temp; 								  								 //温度
int main(void)	
{ 
	delay_init();	    	           //=====延时函数初始化	
	NVIC_Configuration();					 //=====中断优先级分组,其中包含了所有的中断优先级的配置,方便管理和一次性修改。
	uart1_init(115200);	             //=====串口1初始化
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	MPU_Init();					    			 //=====初始化MPU6050
	mpu_dmp_init();								 //=====初始化MPU6050的DMP模式	
  while(1)	
	{
		/*方便我们查看数据的变化,查看姿态角;如果需要看加速度以及角速度可自行修改***/
		mpu_dmp_get_data(&pitch,&roll,&yaw);			//得到姿态角即欧拉角
		temp=MPU_Get_Temperature();								//得到温度值
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据	
			
		printf("pitch=%.2f,roll=%.2f,yaw=%.2f,temp=%.2f\n",pitch,roll,yaw,temp/100);
		/*也可以使用电脑上位机查看,如果没有购买OLED屏幕的客户打开电脑上位机,将波特率设置成9600，使用串口打印到上位机即可查看***/
	} 	
}

