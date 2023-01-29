#include "stm32f10x.h" 
#include "sys.h"
#define LED_pin GPIO_Pin_6
#define LED_GPIO GPIOE

#define LED_TOGGLE LED_Toggle()
void LED_Config(void);
void LED_Toggle(void);
void LED_Toggle2(void);

