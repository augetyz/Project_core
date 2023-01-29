#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"	
#include "led.h"
#include "TIME.h"
#include "mpu6050.h"
#include "motor.h"
#include "bmq.h"
#include "pwm.h"
#include "hc_sr04.h"
#include "GPIO_config.h"
#include "iic_gpio.h"
#include "iic_oled.h"
#include <stdio.h>
#include <stdlib.h>
float Pitch,Roll,Yaw;
extern float Med_Angle;
extern float Vertical_Kp,Vertical_Kd;
extern short gyro[3], accel[3], sensors;
extern const unsigned char BMP1[];
unsigned  char cStr [10];
int main(void)
{		
	unsigned char i;
	delay_init();
	uart_init(115200);
	LED_Init();
	IIC_Init();
	MPU6050_initialize();    
	DMP_Init();
	TIM1_PWM_Init(4800,0);
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	hc_sr04_Config();
	motor_Init();
	OLED_Init();
	OLED_Fill(0xFF);Delay_ms(1000);
	OLED_Fill(0x00);
	TIM4_Getsample_Int(1999,71);
	for(i=0;i<4;i++)
		{
			OLED_ShowCN(32+i*16,0,i);									
		}
	while(1)
	{
		TIM_SetCompare1(TIM1,4000);TIM_SetCompare4(TIM1,4000);
		sprintf ( (char*)cStr, "Pitch: %0.3f",Pitch);
		OLED_ShowStr(0,2,cStr,1);				
		sprintf ( (char*)cStr, "goal angle: %.2f",Med_Angle);
		OLED_ShowStr(0,4,cStr,1);
		sprintf ( (char*)cStr, "Encoderleft: %d",getTIMx_DetaCnt(TIM2));
		OLED_ShowStr(0,6,cStr,1);
		sprintf ( (char*)cStr, "Encoderright: %d",getTIMx_DetaCnt(TIM3));
		OLED_ShowStr(0,7,cStr,1);
	}
	
}
