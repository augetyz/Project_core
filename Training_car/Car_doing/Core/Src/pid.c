#include "pid.h"

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

void pid_doing(Car_status* car)
{
    static int16_t speed_last[4],speed_last_last[4];
    static int16_t speed_sum[4];
    int16_t speed_dert[4];
    uint16_t i=0;
    
    for(i=0;i<4;i++)
    {
        speed_dert[i]=car->goal_speed[i]-car->Car_speed[i]; //���β�ֵ����
        
        speed_sum[i]+=speed_dert[i];                        //�ۼƲ�ֵ����       
        speed_last_last[i]=speed_last[i];                   //���ϴβ�ֵ����
        speed_last[i]=speed_dert[i];                        //�ϴβ�ֵ����
    }
    for(i=0;i<4;i++)//pid�������������ֵ
    {
        car->goal_speed[i] += car->kp[i] * speed_dert[i] + \
                              car->ki[i] * speed_sum[i]  + \
                              car->kd[i] * (speed_dert[i]-speed_last[i]*2+speed_last_last[i]);
    }
    
}

