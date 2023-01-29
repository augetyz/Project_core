#ifndef __USART4_H
#define	__USART4_H


#include "stm32f10x.h"
#include <stdio.h>




void USART4_Config(void);
void Usart4_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart4_SendString( USART_TypeDef * pUSARTx, char *str);
//void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void distance_date_deal(uint8_t *date,int i);
#endif /* __USART_H */
