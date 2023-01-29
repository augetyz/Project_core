#include "LED.h"
void LED_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PA,PD端口时钟
	
	GPIO_InitStructure.GPIO_Pin = LED_pin;				 //LED0-->PA.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(LED_GPIO, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PA.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(LED_GPIO, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
	
}
void LED_Toggle(void)
{
	static u8 sign_led;
	if(sign_led)
	{
		PEout(LED_pin)=1;
		sign_led=0;
	}
	else
	{
		PEout(LED_pin)=0;
		sign_led=1;
	}
}
void LED_Toggle2(void)
{
	static u8 sign_led2;
	if(sign_led2)
	{
		PEout(GPIO_Pin_5)=1;
		sign_led2=0;
	}
	else
	{
		PEout(GPIO_Pin_5)=0;
		sign_led2=1;
	}
}
