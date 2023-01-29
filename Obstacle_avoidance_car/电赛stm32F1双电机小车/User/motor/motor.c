#include "motor.h"


void Motor_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	 //使能PA,PD端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;				 //LED0-->PA.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
	GPIO_SetBits(GPIOE,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);						 
}



void speed_control(int lunzi,int v)
{
	int go;
	v=(v>999)?999:((v<-999)?-999:v);
	if(v<0)
		go=0,v=-v;
	switch(lunzi)
	{
		case 1:
			if(go)
				PEout(11)=0,PEout(12)=1;
			else
				PEout(12)=0,PEout(11)=1;
			TIM_SetCompare1(TIM3,v);break;
		case 2:	
			if(go)
				PEout(13)=0,PEout(14)=1;
			else
				PEout(14)=0,PEout(13)=1;
			TIM_SetCompare2(TIM3,v);break;
	}		
}

