#include "pid.h"
#include <math.h>

float   kp1=2.5,ki1=0,kd1=0;//红
float   kp2=2.5,ki2=0,kd2=0;//绿
float   kp3=10,ki3=0,kd3=0;
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
extern int goal1, 
	goal2;

int x0=0,
	y0=0;
extern float pitch,roll,yaw;
uint8_t goal=1;
void motor_control(int goal_1,int goal_2)
{
	int go1=1,go2=1;
	static int pwm_out1,pwm_out2;
	extern int speed1,speed2;
	int differ_speed1,differ_speed2;
	static int dif_speed_sum1;static int dif_speed_last1,dif_speed_lastlast1;
	static int dif_speed_sum2;static int dif_speed_last2,dif_speed_lastlast2;
	
	speed1=speed1/10;speed2=speed2/10;
//	speed1=speed1;speed2=speed2;speed3=speed3;speed4=speed4;
	differ_speed1=goal_1-speed1;
	differ_speed2=goal_2-speed2;
   
	
	dif_speed_sum1+=differ_speed1;
	dif_speed_sum2+=differ_speed2;

	
	pwm_out1+=kp1*differ_speed1+ki1*dif_speed_sum1+kd1*(differ_speed1-2*dif_speed_last1+dif_speed_lastlast1);
	pwm_out2+=kp2*differ_speed2+ki2*dif_speed_sum2+kd2*(differ_speed2-2*dif_speed_last2+dif_speed_lastlast2);

	
	dif_speed_lastlast1=dif_speed_last1;
	dif_speed_lastlast2=dif_speed_last2;

	
	dif_speed_last1=differ_speed1;
	dif_speed_last2=differ_speed2;

	
	speed_control(Car_Left,pwm_out1*go1);speed_control(Car_Right,pwm_out2*go2);
}
 


void pidsend(void)
{
	switch(goal)
	{
		case 1:printf("%f,%f,%f\n",kp1,ki1,kd1);break;
		case 2:printf("%f,%f,%f\n",kp2,ki2,kd2);break;
        case 3:printf("%f,%f,%f\n",kp3,ki3,kd3);break;
	}
	
}
float pid_date(u8* a)
{
	int l,m,n,b;float k=0;
	for(l=0;a[l]!=':';l++);//1
	for(m=0;a[m]!='.';m++)//4
	{
		if(a[m]=='\n')
		break;
	}
	for(n=0;a[n]!='\n';n++);//8
	for(l++;l<m;l++)
	{		
		k+=(a[l]-0x30)*pow(10,(double)(m-l-1));
	}
	for(b=m++;m<n;m++)
	{		
		k+=(a[m]-0x30)*pow(0.1,(double)(m-b));
	}
	return k;
}
void date_deal(u8 *p)
{
	char order=p[0];
	int v;float k;uint8_t n;
	for(n=0;p[n]!=0x0a;n++);
	switch(order)
	{
		case 'G':
			if(p[2]==0x30)
				switch(goal)
				{
					case 1:goal1=0;break;
					case 2:goal2=0;break;

				}
			else
				if(p[2]==0x31)
					switch(goal)
					{
						case 1:goal1=30;break;
						case 2:goal2=-30;break;

					}
			PAout(11)=1;break;
			
		case 'L':
			goal=p[2]-0x30;
			PAout(11)=1;break;
			
		case 'v':
			if(p[2]==0x2d)
				v=-(int)((p[3]-0x30)*10+(p[4]-0x30));
			else
				v=(p[2]-0x30)*10+(p[3]-0x30);
			switch(goal)
				{
					case 1:goal1=v;break;
					case 2:goal2=v;break;

				}
			PAout(11)=1;break;
			
		case 'p':
			
			k=pid_date(p);
			switch(goal)
			{
				case 1:kp1=k;break;
				case 2:kp2=k;break;
                case 3:kp3=k;break;

			}
			PAout(11)=1;break;
			
		case 'i':
			k=pid_date(p);
			switch(goal)
			{
				case 1:ki1=k;break;
				case 2:ki2=k;break;
                case 3:ki3=k;break;

			}
			PAout(11)=1;break;
			
		case 'd':
			k=pid_date(p);
			switch(goal)
			{
				case 1:kd1=k;break;
				case 2:kd2=k;break;
                case 3:kd1=k;break;
			}
			PAout(11)=1;break;
			
		default:PAout(11)=1;break;
	}
	
}





