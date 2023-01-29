#include "encoder.h"

/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    //使能定时器2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //开启AFIO时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//使能JTAGDisable，即禁用JTAG接口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);   //使能PA端口时钟
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//端口配置
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPD;   //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//根据设定参数初始化GPIOA
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 65535;              //设定计数器自动重装值 
    TIM_TimeBaseStructure.TIM_Prescaler =0;             //预分频器   
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);         //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    TIM_ITRxExternalClockConfig(TIM2,TIM_TS_ETRF);          //配置外部触发，否则不会计数
    TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
    TIM_SetCounter(TIM2, 0);        
    
    TIM_Cmd(TIM2,ENABLE );                                  //使能定时器
}
/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM4(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //使能定时器4的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);   //使能PB端口时钟
	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //浮空输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//根据设定参数初始化GPIOB
	
	
	TIM_TimeBaseStructure.TIM_Period = 65535;              //设定计数器自动重装值 
    TIM_TimeBaseStructure.TIM_Prescaler =0;             //预分频器   
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);         //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    TIM_ITRxExternalClockConfig(TIM4,TIM_TS_ETRF);          //配置外部触发，否则不会计数
    TIM_ETRClockMode1Config(TIM4, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
    TIM_SetCounter(TIM4, 0);        
    
    TIM_Cmd(TIM4,ENABLE );  
}
void TIM3_ETR(void)
{     
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOD,GPIO_Pin_2);                        
    
    //初始化定时器 TIM3   

    TIM_TimeBaseStructure.TIM_Period = 65535; //设定计数器自动重装值 
    TIM_TimeBaseStructure.TIM_Prescaler =0;     //预分频器 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    //中断分组初始化
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
    
    TIM_ITRxExternalClockConfig(TIM3,TIM_TS_ETRF);
    TIM_ETRClockMode2Config(TIM3,TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted,0x00);
    TIM_SetCounter(TIM3, 0);  
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//允许更新中断   
    
    TIM_Cmd(TIM3,ENABLE );
}


//定时器3中断服务程序     
void TIM3_IRQHandler(void)
{ 
     if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //获取中断状态
    { TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
           TIM3->CNT=0;  
        }
}

/**************************************************************************
函数功能：读取编码器脉冲差值
入口参数：TIM_TypeDef * TIMx
返回  值：无
**************************************************************************/
s16 getTIMx_DetaCnt(TIM_TypeDef * TIMx)
{
	s16 cnt;
	cnt = TIMx->CNT-0x7fff;
	TIMx->CNT = 0x7fff;
	return cnt;
}


int Get_Motor_Speed(u8 lunzi)
{
	int speed;
		switch(lunzi)
	{
		
		case 1:
			speed=-getTIMx_DetaCnt(TIM2);
			break;
		case 2:
			speed=getTIMx_DetaCnt(TIM4);
			break;
	}
	return speed;
}


