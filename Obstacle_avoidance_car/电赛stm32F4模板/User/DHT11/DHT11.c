#include "DHT11.h"

void DHT_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// ����GPIO��ʱ��
	RCC_AHB1PeriphClockCmd(DHT11_date_GPIO_CLK, ENABLE); 
	// ѡ��Ҫ���Ƶ�IO��													   
	GPIO_InitStructure.GPIO_Pin =DHT11_date_PIN;	
	// ����IO�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	// ����IO���Ϊ����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	// IO �ڲ�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	// IO �������Ϊ50M
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	// ��ʼ��IO��
	GPIO_Init(DHT11_date_GPIO, &GPIO_InitStructure);
}
void DHT11_start(void)
{
	GPIO_ResetBits(DHT11_date_GPIO,DHT11_date_PIN);
	delay_ms(20);
	GPIO_SetBits(DHT11_date_GPIO,DHT11_date_PIN);
	delay_us(30);
}




