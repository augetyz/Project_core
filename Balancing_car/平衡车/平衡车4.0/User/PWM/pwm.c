 #include "pwm.h"

void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能
	                                                                     	

   //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	

	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
}


void TIM2_PWM_Init(u16 arr,u16 psc){
    GPIO_InitTypeDef     GPIO_InitStrue;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2_Def,ENABLE);  // TIM
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_TIM2_Def,ENABLE);  //GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    
 
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);  // reflect PB3 TIM2_CH2
    
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_TIM2_Def;  //Pin
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIO_TIM2_Def,&GPIO_InitStrue);  // GPIO
    
    TIM_TimeBaseInitStrue.TIM_Period=arr;
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc;
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; 
    TIM_TimeBaseInit(TIM2_Def,&TIM_TimeBaseInitStrue); //TIM
    
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;
    
    TIM2_OC_CH_Init(TIM2_Def,&TIM_OCInitStrue);  //TIM_CH
    
    TIM2_OC_CH_PreloadConfig(TIM2_Def,TIM_OCPreload_Enable); //TIM_CH
    
    TIM_Cmd(TIM2_Def,ENABLE);  //TIM
}
void TIM3_PWM_Init(u16 arr,u16 psc){
    GPIO_InitTypeDef     GPIO_InitStrue;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3_Def,ENABLE);  // TIM
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_TIM3_Def,ENABLE);  //GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_TIM3_Def;  //Pin
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIO_TIM3_Def,&GPIO_InitStrue);  // GPIO
    
    // GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);  // reflect PB3 TIM2_CH2
    
    TIM_TimeBaseInitStrue.TIM_Period=arr;
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc;
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; 
    TIM_TimeBaseInit(TIM3_Def,&TIM_TimeBaseInitStrue); //TIM
    
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;
    
    TIM3_OC_CH_Init(TIM3_Def,&TIM_OCInitStrue);  //TIM_CH
    
    TIM3_OC_CH_PreloadConfig(TIM3_Def,TIM_OCPreload_Enable); //TIM_CH
    
    TIM_Cmd(TIM3_Def,ENABLE);  //TIM
}


void TIM4_PWM_Init(u16 arr,u16 psc){
    GPIO_InitTypeDef     GPIO_InitStrue;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4_Def,ENABLE);  // TIM
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_TIM4_Def,ENABLE);  //GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_TIM4_Def;  //Pin
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIO_TIM4_Def,&GPIO_InitStrue);  // GPIO
    
    // GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);  // reflect PB3 TIM2_CH2
    
    TIM_TimeBaseInitStrue.TIM_Period=arr;
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc;
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; 
    TIM_TimeBaseInit(TIM4_Def,&TIM_TimeBaseInitStrue); //TIM
    
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;
    
    TIM4_OC_CH_Init(TIM4_Def,&TIM_OCInitStrue);  //TIM_CH
    
    TIM4_OC_CH_PreloadConfig(TIM4_Def,TIM_OCPreload_Enable); //TIM_CH
    
    TIM_Cmd(TIM4_Def,ENABLE);  //TIM
}
