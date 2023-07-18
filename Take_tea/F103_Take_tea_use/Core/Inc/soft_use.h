#ifndef _SOFT_USE_H_
#define _SOFT_USE_H_

#define KEY1 GPIO_PIN_2
#define KEY2 GPIO_PIN_8
#define S_KEY1 GPIO_PIN_3
#define S_KEY2 GPIO_PIN_11
#define S_KEY3 GPIO_PIN_4
#define S_KEY4 GPIO_PIN_12



typedef struct 
{
    int step_goal[4];
    int step_use[4];
    int BJ_now[4];
    int sign_now[2];
}BJ_status;


#endif



