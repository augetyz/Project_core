#include "pwm.h"



static void TIM3_Mode_Config(void)
{     
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;	
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	u16 CCR1_Val = 0;                        

	u16 CCR2_Val = 0;                        

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);     
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                    // �����������  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	
    TIM_TimeBaseStructure.TIM_Period = 999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����  
     TIM_TimeBaseStructure.TIM_Prescaler = 71;             
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        //����ʱ�ӷ�Ƶϵ��������Ƶ           
 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ  
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                   
 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;            //����ΪPWMģʽ1 
	
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                       
    TIM_OCInitStructure.TIM_Pulse = CCR1_Val;           //��������ֵ�������������������ֵʱ����ƽ�������� 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ  
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);         //ʹ��ͨ��1  
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);    
 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                       
    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;           //��������ֵ�������������������ֵʱ����ƽ�������� 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ  
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);         //ʹ��ͨ��2  
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 	
 
    TIM_ARRPreloadConfig(TIM3, ENABLE);                         // ʹ��TIM1���ؼĴ���ARR    
	TIM_CtrlPWMOutputs(TIM3, ENABLE); 
	TIM_Cmd(TIM3, ENABLE);  
}

void Motor_Config(void)
{	
	TIM3_Mode_Config();
	Motor_GPIO_Config();
}
