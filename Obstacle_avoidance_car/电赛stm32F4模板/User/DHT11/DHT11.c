#include "DHT11.h"

void DHT_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// 开启GPIO的时钟
	RCC_AHB1PeriphClockCmd(DHT11_date_GPIO_CLK, ENABLE); 
	// 选择要控制的IO口													   
	GPIO_InitStructure.GPIO_Pin =DHT11_date_PIN;	
	// 设置IO口输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	// 设置IO输出为推挽模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	// IO 内部上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	// IO 输出速率为50M
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	// 初始化IO口
	GPIO_Init(DHT11_date_GPIO, &GPIO_InitStructure);
}
void DHT11_start(void)
{
	GPIO_ResetBits(DHT11_date_GPIO,DHT11_date_PIN);
	delay_ms(20);
	GPIO_SetBits(DHT11_date_GPIO,DHT11_date_PIN);
	delay_us(30);
}




