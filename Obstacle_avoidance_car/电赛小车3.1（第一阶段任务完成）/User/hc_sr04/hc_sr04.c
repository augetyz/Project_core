#include "hc_sr04.h"

void hc_sr04_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	 //使能RCC时钟

	GPIO_InitStructure.GPIO_Pin = Echo;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 //浮空输入
	GPIO_Init(hc_sr04_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = Trig;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //浮空输入
	GPIO_Init(hc_sr04_GPIO, &GPIO_InitStructure);
}

void Trig_biu(void)
{
	GPIO_SetBits(hc_sr04_GPIO,Trig);
	
	Delay_us(20);
	
	GPIO_ResetBits(hc_sr04_GPIO,Trig);
}

float get_hc_sr(void)
{
	float s=0;
	int time=0;
	
	Trig_biu();
	while(GPIO_ReadInputDataBit(hc_sr04_GPIO,Echo)==0);
	SysTick_Config(SystemCoreClock/1000000);
	while(GPIO_ReadInputDataBit(hc_sr04_GPIO,Echo)==1)
		{
			while( !((SysTick->CTRL)&(1<<16)) );
			time++;
		}
	s=time;
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
	s=s/2*343/1000/10;
	return s;
}



