#include "pid.h"
#include <stdio.h>
/*********************************************************/
//������������ѣ���С����˳��飻
//���Ǳ�������֣�����ٰ�΢�ּӣ�
//�����񵴺�Ƶ������������Ҫ�Ŵ�
//����Ư���ƴ��壬����������С�⣻
//����ƫ��ظ���������ʱ�����½���
//���߲������ڳ�������ʱ���ټӳ���
//������Ƶ�ʿ죬�Ȱ�΢�ֽ�������
//���������������΢��ʱ��Ӧ�ӳ���
//����������������ǰ�ߺ���ı�һ��
//һ�������������������������ͣ�

//��Ҫ��Ӧ���죬����P��СI��

//��Ҫ��Ӧ��������СP����I��

//�������̫�󣬻�����ϵͳ�𵴣�

//�������̫�󣬻�����ϵͳ�ٶۡ�
/**********************************************************/
extern Car_status car_status;

void pid_doing(Car_status car)
{
    static int16_t speed_last[4],speed_last_last[4];
    static int16_t speed_sum[4],speed[4];
    int16_t speed_dert[4];
    uint8_t i=0;
    
    for(i=0;i<4;i++)
    {
        speed_dert[i]=car.goal_speed[i]-car.Car_speed[i]; //���β�ֵ����
        
        speed_sum[i]+=speed_dert[i];                        //�ۼƲ�ֵ����
        speed_sum[i]=speed_sum[i]>10000?10000:(speed_sum[i]<-10000?-10000:speed_sum[i]);
        speed_last_last[i]=speed_last[i];                   //���ϴβ�ֵ����
        speed_last[i]=speed_dert[i];                        //�ϴβ�ֵ����
    }
    for(i=0;i<4;i++)//pid�������������ֵ
    {
        speed[i] = car.kp[i] * speed_dert[i] + car.ki[i] * speed_sum[i]  + car.kd[i] * (speed_dert[i]-speed_last[i]*2+speed_last_last[i]);
    }

    speed_ctrl(Motor1, speed[0]);
    speed_ctrl(Motor2, speed[1]);
    speed_ctrl(Motor3, speed[2]);
    speed_ctrl(Motor4, speed[3]);
}

void pid_do(biu speed_goal,biu speed_now)
{
    static int16_t speed_last[4],speed_last_last[4];
    static int16_t speed_sum[4],speed[4];
    int16_t speed_dert[4];
    uint8_t i=0;
    static PID pid;//�ú��������������ʹ�ã����Բ�ʹ��ȫ�ֱ�������ʹ��static���Ʒ���
    pid.kp[0]=0.6;    pid.kp[1]=0.5;    pid.kp[2]=0.6;   pid.kp[3]=0.46;
    pid.ki[0]=0.1;   pid.ki[1]=0.1;   pid.ki[2]=0.12; pid.ki[3]=0.1;
    pid.kd[0]=0;   pid.kd[1]=0;   pid.kd[2]=0.0;   pid.kd[3]=0.0;
    
    for(i=0;i<4;i++)
    {
        speed_dert[i]=speed_goal.date[i]-speed_now.date[i]; //���β�ֵ����
        
        speed_sum[i]+=speed_dert[i];                        //�ۼƲ�ֵ����    
        speed_sum[i]=speed_sum[i]>10000?10000:(speed_sum[i]<-10000?-10000:speed_sum[i]);  //�����޷�      
        speed_last_last[i]=speed_last[i];                   //���ϴβ�ֵ����
        speed_last[i]=speed_dert[i];                        //�ϴβ�ֵ����
    }
    for(i=0;i<4;i++)//pid�������������ֵ
    {
        speed[i] = pid.kp[i] * speed_dert[i] + \
                              pid.ki[i] * speed_sum[i]  + \
                              pid.kd[i] * (speed_dert[i]-speed_last[i]*2+speed_last_last[i]);
    }

    speed_ctrl(Motor1, speed[0]);
    speed_ctrl(Motor2, speed[1]);
    speed_ctrl(Motor3, speed[2]);
    speed_ctrl(Motor4, speed[3]);

}
