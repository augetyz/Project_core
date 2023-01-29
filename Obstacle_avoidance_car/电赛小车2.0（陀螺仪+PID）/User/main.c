#include "stm32f10x.h"  
#include "pwm.h"
#include "motor.h"
#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "usart.h"
#include <stdio.h>
#include "pid.h"
#include "encoder.h"
#include "TiMbase.h" 
#include "mpu6050.h"
#include "mpuiic.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
/*
PWM  
LEFT PA6  RIGHT   PA7

ENCODER  
LEFT   PA15  PB3
RIGHT  PB6  PB7

方向
LEFT  PB0  PB1
RIGHT PB4  PB5

舵机 PA1
*/
//串口：TX：PB10 RX：PB11
//MPU6050 :SCL: PC6  SDA: PC7

/****  ***全局变量声明调用******************/
extern int biu,mode_car;
extern u8 biubiu[3];
extern float kp,kd,ki;
extern int goal1, goal2,speed1,speed2;;
float pitch,roll,yaw; 								  			 //欧拉角(姿态角)
extern float distance1,distance2;
uint8_t sign=1;
void set_distance(float a,float b);
int main()
{
    /********变量定义************************/
    uint16_t i=0;
    
    
    /*******板子外设初始化*******************/
	USART_Config();
    LED_Config();
	printf("你是猪");  	
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
    Motor_Config();	
    sg_biu(100);
    Motor_GPIO_Config();
    MPU_Init();					    			 //=====初始化MPU6050
	mpu_dmp_init();								 //=====初始化MPU6050的DMP模式	

    /*******开始了**************************/
    PEout(5)=1;PEout(6)=1;
    while(1)
    {
        mpu_dmp_get_data(&pitch,&roll,&yaw);
        PEout(5)=0;
        i++;
        if(i>4000)
           break;
        printf("%d,%d,%d,%d,%.2f\n",speed1,speed2,goal1,goal2,yaw);
    }
    PEout(5)=1;
    BASIC_TIM_Init();
    mpu_dmp_get_data(&pitch,&roll,&yaw);
    while(1)
    {
        mpu_dmp_get_data(&pitch,&roll,&yaw);
        printf("%d,%d,%d,%d,%.2f,%.2f,%.2f\n",speed1,speed2,goal1,goal2,yaw,distance1,distance2);
        switch(mode_car)
        {
            case 1:set_distance(500,500);break;
            case 2:set_distance(145,-145);break;
            case 3:set_distance(500,500);break;
            case 4:set_distance(-145,145);break;
            case 5:set_distance(500,500);break;
            case 6:mode_car=0;break;
        }      
    }
    
}

void set_distance(float a,float b)
{
    int8_t m,n;
    m=a>0?1:0;
    n=b>0?1:0;
    if(sign)
        {
            if(m)
            {
                if(distance1<a-5)
                    goal1=m*10;
                else
                    goal1=0,sign=0;
            }
            else
            {
                if(distance1>a+5)
                    goal1=-1*10;
                else
                    goal1=0,sign=0;
            }
            if(n)
            {
                if(distance2<b-5)
                    goal2=n*10;
                else
                    goal2=0,sign=0;
            }
            else
            {
                if(distance2>b+5)
                    goal2=-1*10;
                else
                    goal2=0,sign=0;
            }
        }
    else
        {
            if(m)
            {              
                if(distance1>a+10)
                    goal1=-2*m;
                else
                    goal1=0,sign=1;
            }
            else
            {
                if(distance1<a-10)
                    goal1=-2*-1;
                else
                    goal1=0,sign=1;
            }
            if(n)
            {
                if(distance2>b+10)
                    goal2=-2*n,sign=1;
                else
                    goal2=0;
            }
            else
            {
                if(distance2<b-10)
                goal2=-2*-1,sign=1;
                else
                    goal2=0;
            }
        }
}