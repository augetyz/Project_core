 #include "pwm.h"

static void TIM1_GPIO_Config(void)
{  
      GPIO_InitTypeDef GPIO_InitStructure;  
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);     
 
      GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_11;   
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                    // �����������  
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
      GPIO_Init(GPIOA, &GPIO_InitStructure);
  }
static void TIM1_Mode_Config(int arr,int psc)
{        
      TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;        
      TIM_OCInitTypeDef  TIM_OCInitStructure;       
      u16 CCR1_Val = 0;                        
      u16 CCR4_Val = 0; 
  
     TIM_TimeBaseStructure.TIM_Period = arr;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����  
     TIM_TimeBaseStructure.TIM_Prescaler = psc;             
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
     TIM_OCInitStructure.TIM_Pulse = CCR4_Val;        //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM  
     TIM_OC4Init(TIM1, &TIM_OCInitStructure);        //ʹ��ͨ��4 
     TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  
 
     TIM_ARRPreloadConfig(TIM1, ENABLE);                         // ʹ��TIM1���ؼĴ���ARR    
	 TIM_CtrlPWMOutputs(TIM1, ENABLE); 
	 TIM_Cmd(TIM1, ENABLE);  
} 
void TIM1_PWM_Init(u16 arr,u16 psc)
{       
     TIM1_GPIO_Config();     
     TIM1_Mode_Config(arr,psc);      
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
