#include "stm32f10x.h"  
#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "usart.h"
#include <stdio.h>
#include "TiMbase.h" 

main()
{
	USART_Config();
	delay_init();
	printf("ƒ„ «÷Ì");
	BASIC_TIM_Init();
	while(1)
	{
		
	}
}




