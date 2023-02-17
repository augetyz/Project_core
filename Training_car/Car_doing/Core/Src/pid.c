#include "pid.h"

/*********************************************************/
//参数整定找最佳，从小到大顺序查；
//先是比例后积分，最后再把微分加；
//曲线振荡很频繁，比例度盘要放大；
//曲线漂浮绕大湾，比例度盘往小扳；
//曲线偏离回复慢，积分时间往下降；
//曲线波动周期长，积分时间再加长；
//曲线振荡频率快，先把微分降下来；
//动差大来波动慢。微分时间应加长；
//理想曲线两个波，前高后低四比一；
//一看二调多分析，调节质量不会低；

//若要反应增快，增大P减小I；

//若要反应减慢，减小P增大I；

//如果比例太大，会引起系统震荡；

//如果积分太大，会引起系统迟钝。
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
        speed_dert[i]=car->goal_speed[i]-car->Car_speed[i]; //本次差值计算
        
        speed_sum[i]+=speed_dert[i];                        //累计差值计算       
        speed_last_last[i]=speed_last[i];                   //上上次差值计算
        speed_last[i]=speed_dert[i];                        //上次差值计算
    }
    for(i=0;i<4;i++)//pid参数作用于输出值
    {
        car->goal_speed[i] += car->kp[i] * speed_dert[i] + \
                              car->ki[i] * speed_sum[i]  + \
                              car->kd[i] * (speed_dert[i]-speed_last[i]*2+speed_last_last[i]);
    }
    
}

