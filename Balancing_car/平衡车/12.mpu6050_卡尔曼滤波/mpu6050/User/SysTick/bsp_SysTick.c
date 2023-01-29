
//SysTick_Config主要用来配置中端向量，重置STK_VAL寄存器，配置SysTick时钟为AHB 
#include "bsp_SysTick.h"
 

void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock/1000))	// ST3.5.0库版本SystemCoreClock/10不能超过16777216
	{ 
		/* Capture error */ 
		while (1);
	}
		// 关闭滴答定时器  
	//SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
	//开启
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
}

