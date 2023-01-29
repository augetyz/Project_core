#include "stm32f4xx.h"
#include "led.h"  
#include "key.h" 
#include "GPIO.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>
#include "ili9341_lcd.h"
#include "fonts.h"	
#include "bsp_spi_flash.h"


int main(void)
{
	Debug_USART_Config();
	SysTick_Init();	
	Key_GPIO_Config();
	ILI9341_Init();
	ILI9341_GramScan ( 6 );
	ColorLED_Config();
	LCD_SetFont(&Font16x32);
	LCD_SetColors(BLACK,BLACK);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* ÇåÆÁ£¬ÏÔÊ¾È«ºÚ */	
	SetRGBColor(898985);
	picture_show(240,240,gImage_ustic);
	while(1)
	{
		
	}
}


