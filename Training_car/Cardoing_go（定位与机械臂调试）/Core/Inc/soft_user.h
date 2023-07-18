#ifndef _SOFT_USER_H_
#define _SOFT_USER_H_

#include "main.h"
#include <stdio.h>

void delay(uint32_t time);
void delay_us(uint32_t time);
void Usart_SendString(uint8_t *str);
void clearArray(uint8_t * array,uint16_t length);

void V831_QuickMark_do(void);
void V831_Color_Find_do(void);
void V831_die(void);

void V831_2_color_circle_do(void);
void V831_2_black_line_Find_do(void);
void V831_2_die(void);

#endif // !_SOFT_USER_H_
