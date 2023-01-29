#ifndef _DHT11_H
#define _DHT11_H

#include "stm32f4xx.h"
#include "./GPIO/GPIO.h" 
#include "./delay/delay.h" 

#define DHT11_date_GPIO                GPIOA
#define DHT11_date_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define DHT11_date_PIN                 GPIO_Pin_1



#endif
