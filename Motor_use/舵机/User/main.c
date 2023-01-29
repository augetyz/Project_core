#include "stm32f10x.h"
#include "systick_delay.h"
#include "pwm4.h"
#include "LED.h"



main()
{
    SysTick_Init();
    TIM4_PWM_Init(999,1439);
		
		while(1)
		{
			TIM_SetCompare3(TIM4,50);
			TIM_SetCompare4(TIM4,50);
			SysTick_Delay_Ms(2000);
			TIM_SetCompare3(TIM4,100);
			TIM_SetCompare4(TIM4,100);
			SysTick_Delay_Ms(2000);
		}

}

	
	
	



