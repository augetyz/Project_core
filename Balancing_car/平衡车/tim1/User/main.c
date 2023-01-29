#include "stm32f10x.h"  
#include "stm32f10x_tim.h"
static void TIM1_GPIO_Config(void)
{  
      GPIO_InitTypeDef GPIO_InitStructure;  
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);     
 
      GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9| GPIO_Pin_10 | GPIO_Pin_11;   
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                    // �����������  
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
      GPIO_Init(GPIOA, &GPIO_InitStructure);
  }
static void TIM1_Mode_Config(void)
{        
      TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;        
      TIM_OCInitTypeDef  TIM_OCInitStructure;       
      u16 CCR1_Val = 0;                
      u16 CCR2_Val = 0;       
      u16 CCR3_Val = 0;        
      u16 CCR4_Val = 0; 
  
     TIM_TimeBaseStructure.TIM_Period = 999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����  
     TIM_TimeBaseStructure.TIM_Prescaler = 71;             
     TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;        //����ʱ�ӷ�Ƶϵ��������Ƶ           
 
     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ  
     TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);                   
 
     TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;            //����ΪPWMģʽ1   
     TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;               
    // TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;         
     TIM_OCInitStructure.TIM_Pulse = CCR1_Val;           //��������ֵ�������������������ֵʱ����ƽ�������� 
     TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ  
     TIM_OC1Init(TIM1, &TIM_OCInitStructure);         //ʹ��ͨ��1  
     TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  
 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   
    TIM_OCInitStructure.TIM_Pulse = CCR2_Val;          //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM   
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);          //ʹ��ͨ��2 
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);          
 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   
    TIM_OCInitStructure.TIM_Pulse = CCR3_Val;        //����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM   
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);         //ʹ��ͨ��3   
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);    
 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_Pulse = CCR4_Val;        //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM  
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);        //ʹ��ͨ��4 
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  
 
    TIM_ARRPreloadConfig(TIM1, ENABLE);                         // ʹ��TIM1���ؼĴ���ARR    
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 
	TIM_Cmd(TIM1, ENABLE);  
} 
void TIM1_PWM_Init(void)
{       
     TIM1_GPIO_Config();     
     TIM1_Mode_Config();      
}
main()
{
	int i,n;
	TIM1_PWM_Init();
	while(1)
	{
		TIM_SetCompare1(TIM1,500);
		TIM_SetCompare4(TIM1,500);
		for(i=0;i<10000;i++)
		{
			for(n=0;n<100;n++);
		}
		TIM_SetCompare1(TIM1,100);
		TIM_SetCompare4(TIM1,100);
		for(i=0;i<10000;i++)
		{
			for(n=0;n<100;n++);
		}
	}
}
