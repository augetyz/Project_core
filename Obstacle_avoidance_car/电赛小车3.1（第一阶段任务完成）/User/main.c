/*
这一次使用hi229作为IMU,使用了串级pid，实现了角度自由旋转
时序问题通过改善陀螺仪读取问题得到较好改变。
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
//MPU6050 :SCL: PC6  SDA: PC7(已取消)
//HI229:使用串口2：TX：PA2 RX：PA3 



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
#include "ch_serial.h"
#include "usart4.h"
#include "hc_sr04.h"

#define car_stop_part       1
#define car_go_part         2
#define car_angle_part      3
#define car_circle_part     4
#define car_distance_part   5

/****  ***全局变量声明调用******************/

extern int goal1, goal2,speed1,speed2;
extern raw_t raw;
extern float distance1,distance2;
extern float kp3,ki3,kd3,kp1,ki1,kd1,kp2,ki2,kd2;
int distance_sign=0;
float distance;

int goal_out;
int sign;

void date_uploading(void);
void set_distance(float a,float b);
float dert(float a,float b);
uint8_t angle_set(float goal_angle);

int main()
{
    /********变量定义************************/
    float yaw=0;
    int distance_sign=1;
    int distance_back;
    
    /*******板子外设初始化*******************/
	USART_Config();
    LED_Config();
	printf("你是猪");  	
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
    Motor_Config();
    sg_biu(100);
    Motor_GPIO_Config();
    uart2_init();
    hc_sr04_Config();
    
    /*******开始了**************************/
    PEout(5)=1;PEout(6)=1;    
    BASIC_TIM_Init();
    distance=get_hc_sr();
    distance_back=distance*10;
    printf("%d\n",distance_back);
    sign=1;
    while(1)
    {
              
        switch(sign)
        {
            case 1:
                if(distance<40)
                {                    
                    yaw=45;sign=2;                   
                    distance1=distance2=0;
                    distance_sign=0;
                }
                goal1=goal2=60;
                angle_set(yaw);
                break;
            case 2:
                if((distance1+distance2)/2>=500)
                {
                    yaw=-45;sign=3;
                    distance1=distance2=0;
                }
                goal1=goal2=30;
                angle_set(yaw);
                break;
            case 3:
                if((distance1+distance2)/2>=500)
                {
                    yaw=0;sign=4;
                    distance_sign=1;
                    distance=distance1=distance2=0;
                }
                goal1=goal2=30;
                angle_set(yaw);
                break;
            case 4:
                if(distance<30&&distance!=0)
                {
                    yaw=-45;sign=5;
                    distance1=distance2=0;
                    distance_sign=0;
                }
                goal1=goal2=10;
                angle_set(yaw);
                break;  
            case 5:
                if((distance1+distance2)/2>=400)
                {
                    yaw=45;sign=6;
                    distance1=distance2=0;
                }
                goal1=goal2=30;
                angle_set(yaw);
                break;      
            case 6:
                if((distance1+distance2)/2>=400)
                {
                    yaw=135;sign=7;
                    distance1=distance2=0;
                }
                goal1=goal2=30;
                angle_set(yaw);
                break; 
            case 7:
                if((distance1+distance2)/2>=400)
                {
                    yaw=-135;sign=8;
                    distance1=distance2=0;
                }
                goal1=goal2=30;
                angle_set(yaw);
                break;
            case 8:
                if((distance1+distance2)/2>=650)
                {
                    yaw=180;sign=9; 
                    distance_sign=1;
                    distance=distance1=distance2=0;
                } 
                else                
                    goal1=goal2=30;
                angle_set(yaw);
                break;
            case 9:
                if(distance<40&&distance!=0)
                {
                    yaw=-135;sign=10;
                    distance1=distance2=0;
                    distance_sign=0;
                }
                goal1=goal2=10;
                angle_set(yaw);
                break; 
            case 10:
                if((distance1+distance2)/2>=400)
                {
                    yaw=135;sign=11;
                    distance1=distance2=0;
                }
                goal1=goal2=30;
                angle_set(yaw);
                break;
            case 11:
                if((distance1+distance2)/2>=400)
                {
                    yaw=180;sign=12;
                    distance_sign=0;
                    distance=distance1=distance2=0;
                }
                goal1=goal2=30;
                angle_set(yaw);
                break;
            case 12:
                if((distance1+distance2)/2>=distance_back-80)
                {
                    yaw=180;
                    distance_sign=0;
                    goal1=goal2=0;
                }
                else
                    goal1=goal2=30;
                angle_set(yaw);
                break;
        }
//        date_uploading();       
        if(distance_sign)
        {
            if((distance1+distance2)/2>10)
            {
                distance=get_hc_sr(); 
                distance1=distance2=0;                
            }
        }
        Delay_ms(1);
    }    
    
}

void set_distance(float a,float b)
{
    
    ;
}

float dert(float a,float b)
{
    float c=a-b;
    if(c>0&&c<=180)
    {
        return c;
    }
    else {
        if(c>=-180&&c<=0)
            return c;
        else
            if(c<-180)
                return 360+c;
            else
                return c-360;
    }
}
uint8_t angle_set(float goal_angle)
{
    float angle_dert;
    static float angle_dert_sum,angle_dert_last;
    angle_dert=dert(goal_angle,raw.imu[0].eul[2]);
    if(angle_dert>-1&&angle_dert<1)
    {
        goal_out=0;
        return 1;
    }
    else
    {
        
            goal_out=angle_dert*kp3+angle_dert_sum*ki3+(angle_dert-angle_dert_last)*kd3;
            goal_out=goal_out>30?30:(goal_out>-30?goal_out:-30);        
       
    }
    angle_dert_sum+=angle_dert;
    angle_dert_last=angle_dert;
    return 0;
}
void date_uploading(void)
{
    printf("%d,%d,%d,%d,%.2f,%.2f,%.2f,%.2f\n",goal1, goal2,speed1,speed2,distance1,distance2,raw.imu[0].eul[2],distance);

}


uint8_t car_go_distance(int distance,float yaw)
{
    if(distance1>=1000||distance2>=1000)
                {                    
                    distance1=0;distance2=0;
                }
                goal1=goal2=30;
                angle_set(yaw);
    return 1;
}











