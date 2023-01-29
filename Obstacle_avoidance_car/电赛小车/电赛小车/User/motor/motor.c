#include "motor.h"


void Motor_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;				 //LED0-->PA.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5);						 //PA.8 �����
    
}

void speed_control(int lunzi,int v)
{
	int go=0;
	v=(v>999)?999:((v<-999)?-999:v);
	if(v<0)
		go=1,v=-v;
	switch(lunzi)
	{
		case Car_Left:
			if(go)
            {
				TIM_SetCompare1(TIM3,v);
                Car_Left_GO;
            }
			else
            {
				TIM_SetCompare1(TIM3,v);
                Car_Left_Back;
            }
            break;
		case Car_Right:
			if(go)
            {
                TIM_SetCompare2(TIM3,v);
                Car_Right_GO;
            }
			else
            {
                TIM_SetCompare2(TIM3,v);
                Car_Right_Back;
            }
            break;
		
	}
	
	
}



