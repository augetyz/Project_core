//////////////////////////////////////////////////////////////////////////////////	 
//���߷�ʽ
//ʹ��STM32F103C8T6
//1.3"I2C OLED��Ļ
//I2C OLED��Ļ��SDA:PA7	SCL:PA5
//MPU6050���ߣ�SDA:PB7	SCL:PB6   
//����3.3V����
								  
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
	MPU6050_initialize();     //=====MPU6050��ʼ��	
	DMP_Init();
	
	
	TIM2_Getsample_Int(1999,719);		//50ms����ʱ�ж�


	while(1)
	{	
		delay_ms(999);

	
		sprintf((char *)string,"Temp:%.2f",Read_Temperature());//�¶���ʾ
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
