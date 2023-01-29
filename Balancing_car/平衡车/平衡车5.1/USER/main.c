#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"		
#include "led.h"
#include "TIME.h"
#include "mpu6050.h"
#include "OLED.h"
#include "GPIO_config.h"
#include "hc_sr04.h"
#include <stdio.h>
#include <stdlib.h>
#include "bmq.h"
#include "motor.h"
#include "GPIO_config.h"
float Pitch,Roll,Yaw,distance;
extern int i=0;
static int kp=100;
static int kd;
static int Y,X1,X=0;
//float Read_Temperature();

int main(void)
{	

	//u8 string[10] = {0};	
	delay_init();
	USART_Config();printf("串口ok/n");
	LED_Init();printf("LEDok/n");
	//hc_sr04_Config();
	//OLED_Init();
	//OLED_ShowString(40,0,(u8*)"biubiu",12);
	//OLED_Refresh();
	motor_Init();printf("电机ok/n");
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();printf("编码器ok/n");
	TIM1_PWM_Init(4800,0);printf("pwm ok/n");
	IIC_Init();printf("iic ok/n");
	MPU6050_initialize();  printf("mpu6050 ok/n");   //=====MPU6050初始化	
	DMP_Init();printf("DMP ok/n");
	
	TIM4_Getsample_Int(1999,143);printf("定时器 ok/n");		//10ms任务定时中断

	while(1)
	{	
		X1=Y;
		Y=(int)Pitch;
		X=Y-X1;
		if(Y>0&&Y<90)
			{
					motorA(-1);motorB(-1);
					
					speed_A(kp*Y+kd*X);speed_B(kp*Y+kd*X);					
			}
			if(Y<0&&Y>-90)
			{
				Y=-Y;
				motorA(1);motorB(1);	
				speed_A(kp*Y+kd*X);speed_B(kp*Y+kd*X);
			}

	
		/*sprintf((char *)string,"Pitch:%.2f",Pitch);//温度显示
		OLED_ShowString(34,12,string,12);
		delay_ms(100);

		OLED_ShowChinese(96,12,0,16);	
	    sprintf((char *)string,"Distance:%.2f",distance);//0300
		OLED_ShowString(34,26,string,12);
		sprintf((char *)string,"Roll :%.2f",Roll);//0300
		OLED_ShowString(34,40,string,12);
		sprintf((char *)string,"Yaw  :%.2f",Yaw);//0300
		OLED_ShowString(34,52,string,12);
		OLED_Refresh();*/
	}
}
void DEBUG_USART1_IRQHandler(void)
{
	//int kp,X,Y,kd;
	char i; 
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
		{
			i=USART_ReceiveData(DEBUG_USARTx);
			switch(i)
			{
				case 'a':kp+=1; break;
				case 'b':kd+=1; break;
				case 'x':kp+=10; break;
				case 'y':kp-=10; break;
				case 'c':kp-=1; break;
				case 'd':kd-=1; break;
				case 'e':printf("kp:%d kd:%d\n",kp,kd);break;
				case 'f':printf("X:%d Y:%d\n",X,Y);break;
			}
		}
		
}


