#include "motor.h"

void motorA(int g)
{
	switch(g)
	{
		case 0:GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13);break;
		case 1:GPIO_SetBits(GPIOB,GPIO_Pin_12);GPIO_ResetBits(GPIOB,GPIO_Pin_13);break;
		case -1:GPIO_SetBits(GPIOB,GPIO_Pin_13);GPIO_ResetBits(GPIOB,GPIO_Pin_12);break;
	}
}
void motorB(int g)
{
	switch(g)
	{
		case 0:GPIO_SetBits(GPIOB,GPIO_Pin_14|GPIO_Pin_15);break;
		case 1:GPIO_SetBits(GPIOB,GPIO_Pin_15);GPIO_ResetBits(GPIOB,GPIO_Pin_14);break;
		case -1:GPIO_SetBits(GPIOB,GPIO_Pin_14);GPIO_ResetBits(GPIOB,GPIO_Pin_15);break;
	}
}
void speed_A(int g)
{
	TIM_SetCompare4(TIM1,g);
}
void speed_B(int g)
{
	TIM_SetCompare1(TIM1,g);
}
void motor(int A,int B)
{
	if(A>=0)
	{
		motorA(-1);
		speed_A(A);	
	}
	else
	{
		A=-A;
		motorA(1);
		speed_A(A);
	}
	if(B>=0)
	{
		motorB(-1);
		speed_B(B);	
	}
	else
	{
		B=-B;
		motorB(1);
		speed_B(B);
	}
}




