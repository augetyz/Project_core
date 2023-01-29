#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"
#define delay_ms(x) Delay_ms(x);
#define delay_us(x) Delay_us(x);
void delay_init(void);

void Delay_ms(uint32_t ms);
void Delay_us(uint32_t us);
#endif





























