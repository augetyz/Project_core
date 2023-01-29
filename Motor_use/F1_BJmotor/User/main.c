#include "stm32f10x.h"  
#include "TIM_M_S.h"
#include "sys.h"
#include "LED.h"
#include "key.h"
#include "exti.h"
#include "delay.h"
#include "usart.h"
#include "BJ_Motor.h"
int main(void)
{    
    LED_Config();
	KEY_Init();
//  USART_Config();
    Motor_Config();
    BJ1_Enable;
    BJ2_Enable;
    BJ1_1;
    BJ2_1;
    
    Pulse_output2(60,3210);
    Pulse_output1(240,3200);
    while(1)
    { 
        if(KEY0 == 0)
        {
            delay_ms(10);//消抖
            if(KEY0==0)	 //按键KEY1
            {				 
                Pulse_output2(60,3210);
                Pulse_output1(240,3200);
                delay_ms(1000); 
            }
            
        }
        if(KEY1 == 0)
        {
            delay_ms(10);//消抖
            if(KEY1==0)	 //按键KEY1
            {				 
                BJ1_toggle;
                BJ2_toggle;
            }
        }
        Delay_ms(1);
    }
	
}


