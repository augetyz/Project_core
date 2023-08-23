#include "delay.h"
#include "usart.h"
#include "ch_serial.h"
 
/************************************************
 ALIENTEKս��STM32������ʵ��
 ����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

/************************************************
 �������˵���
 ���Ե���ģ�飺 HI226 HI229
 ���ڽ�����������
 ������ֻ��ѧϰʹ�ã��۲��������������������;
 ����2��������HI226������HI229������
 ����1������2�ɹ����յ������ݴ�ӡ���ն���
 ������ն�һ��ָ����PC���ϴ��ڵ�������
 ������http://www.hipnuc.com
************************************************/


static raw_t raw = {0};             /* IMU stram read/control struct */
static uint8_t decode_succ;         /* 0: no new frame arrived, 1: new frame arrived */


int main(void)
{		
    delay_init();	    	                               //��ʱ������ʼ��	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);        //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	                                   //���ڳ�ʼ��Ϊ115200

    while(1)
    {
        if(decode_succ)
        {
            decode_succ = 0;
            delay_ms(800);
            ch_dump_imu_data(&raw);
        }
	}	 
}

void USART2_IRQHandler(void)
{
	uint8_t ch;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        ch = USART_ReceiveData(USART2);	
    }
    
    /* decode each byte */
    decode_succ = ch_serial_input(&raw, ch);
} 
