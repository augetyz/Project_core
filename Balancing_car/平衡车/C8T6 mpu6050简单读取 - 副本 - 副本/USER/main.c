//////////////////////////////////////////////////////////////////////////////////	 
//接线方式
//使用STM32F103C8T6
//1.3"I2C OLED屏幕
//I2C OLED屏幕：SDA:PA7	SCL:PA5
//MPU6050接线：SDA:PB7	SCL:PB6   
//都是3.3V供电
								  
//////////////////////////////////////////////////////////////////////////////////
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"		
#include "led.h"
#include "TIME.h"
#include "mpu6050.h"
#include "OLED.h"

#include <stdio.h>
#include <stdlib.h>
float Pitch,Roll,Yaw;
//float Read_Temperature();

int main(void)
{	
	u8 string[10] = {0};	
	delay_init();
	uart_init(115200);
//	LED_Init();
	
	
	OLED_Init();
	OLED_ShowString(40,0,(u8*)"MPU6050",12);
	OLED_Refresh();
	
	IIC_Init();
	MPU6050_initialize();     //=====MPU6050初始化	
	DMP_Init();
	
	
	TIM2_Getsample_Int(1999,719);		//50ms任务定时中断


	while(1)
	{	
		delay_ms(999);

	
		sprintf((char *)string,"Temp:%.2f",Read_Temperature());//温度显示
		OLED_ShowString(34,12,string,12);
		delay_ms(100);

		OLED_ShowChinese(96,12,0,16);	
	    sprintf((char *)string,"Pitch:%.2f",Pitch);//0300
		OLED_ShowString(34,26,string,12);
		sprintf((char *)string,"Roll :%.2f",Roll);//0300
		OLED_ShowString(34,40,string,12);
		sprintf((char *)string,"Yaw  :%.2f",Yaw);//0300
		OLED_ShowString(34,52,string,12);
		OLED_Refresh();
	}
}
