 #include "GPIO_config.h"
 #include "systick_delay.h"
void LED_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
 	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);	 //使能RCC时钟

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOC, &GPIO_InitStructure);					 //初始化输出
		
		GPIO_ResetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14);// 输出低电平
		//GPIO_SetBits(GPIOC,GPIO_Pin_14);//高电平
}
void motor_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
 	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	 //使能RCC时钟

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);					 //初始化输出
		
		GPIO_ResetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);// 输出低电平
		//GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15);
}
void ledbiu(void)
{
	int i;
	for(i=0;i<3;i++)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay_ms(1000);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}
}

