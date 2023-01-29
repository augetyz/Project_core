#include "encoder.h"

/**************************************************************************
�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    //ʹ�ܶ�ʱ��2��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //����AFIOʱ��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//ʹ��JTAGDisable��������JTAG�ӿ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);   //ʹ��PA�˿�ʱ��
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	//�˿�����
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPD;   //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//�����趨������ʼ��GPIOA
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 65535;              //�趨�������Զ���װֵ 
    TIM_TimeBaseStructure.TIM_Prescaler =0;             //Ԥ��Ƶ��   
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);         //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    TIM_ITRxExternalClockConfig(TIM2,TIM_TS_ETRF);          //�����ⲿ���������򲻻����
    TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
    TIM_SetCounter(TIM2, 0);        
    
    TIM_Cmd(TIM2,ENABLE );                                  //ʹ�ܶ�ʱ��
}
/**************************************************************************
�������ܣ���TIM4��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM4(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //ʹ�ܶ�ʱ��4��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);   //ʹ��PB�˿�ʱ��
	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //��������
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//�����趨������ʼ��GPIOB
	
	
	TIM_TimeBaseStructure.TIM_Period = 65535;              //�趨�������Զ���װֵ 
    TIM_TimeBaseStructure.TIM_Prescaler =0;             //Ԥ��Ƶ��   
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);         //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    TIM_ITRxExternalClockConfig(TIM4,TIM_TS_ETRF);          //�����ⲿ���������򲻻����
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
    
    //��ʼ����ʱ�� TIM3   

    TIM_TimeBaseStructure.TIM_Period = 65535; //�趨�������Զ���װֵ 
    TIM_TimeBaseStructure.TIM_Prescaler =0;     //Ԥ��Ƶ�� 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
    
    TIM_ITRxExternalClockConfig(TIM3,TIM_TS_ETRF);
    TIM_ETRClockMode2Config(TIM3,TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted,0x00);
    TIM_SetCounter(TIM3, 0);  
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��������ж�   
    
    TIM_Cmd(TIM3,ENABLE );
}


//��ʱ��3�жϷ������     
void TIM3_IRQHandler(void)
{ 
     if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //��ȡ�ж�״̬
    { TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
           TIM3->CNT=0;  
        }
}

/**************************************************************************
�������ܣ���ȡ�����������ֵ
��ڲ�����TIM_TypeDef * TIMx
����  ֵ����
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


