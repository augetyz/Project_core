#include "control.h"	
#include "timer.h"
#include "key.h"
/**************************************************************************
函数功能：所有的控制代码都在这里面
         5ms外部中断由MPU6050的INT引脚触发
         严格保证采样和数据处理的时间同步			 
**************************************************************************/
float velocity1,velocity2;
extern int xunji_error;
extern float Roll,Pitch,Yaw; 
struct PID pid_speed={10,50,0};//10 40
struct PID pid_Position={10,0,300};//10 40
struct PID pid_Yaw_turn={3,0,35};//本为60，因为滤波除10，所以为了使原始数据不变，乘10
struct PID pid_Yaw={50,0,0};
struct PID pid_gyro_turn={0,0,1};
struct PID pid_xunji_turn={130,0,0};
int dusu,Left_speed=0,right_speed=0,Turn_Pwm=0;
int Encoder_Left,Encoder_Right,Position_Left,Position_Right;  
int turn=0,medicine_flag=1;
int turn_cnt,turnleft_90=0,turnright_90=0,turnleft_180=0,turnleft90_flag=0,turnright90_flag=0,turnleft180_flag=0;
int xunji=1,stop_cnt=0,stopflag=1;
int EXTI15_10_IRQHandler(void) 
{    
	static int Voltage_Temp,Voltage_Count,Voltage_All;		//电压测量相关变量
	static u8 Flag_Target;																//控制函数相关变量，提供10ms基准
	static int Flag_Yaw=0;
	           					//左右编码器的脉冲计数
	int Balance_Pwm;		  					//平衡环PWM变量，速度环PWM变量，转向环PWM变
	if(INT==0)		
	{   
		EXTI->PR=1<<12;                           					//清除中断标志位   
		Flag_Target=!Flag_Target;
		Get_Angle(Way_Angle);                     					//更新姿态，5ms一次，更高的采样频率可以改善卡尔曼滤波和互补滤波的效果
		Encoder_Left=-Read_Encoder(2);            					//读取左轮编码器的值，前进为正，后退为负
		Encoder_Right=-Read_Encoder(4);           					//读取右轮编码器的值，前进为正，后退为负
		if(delay_flag==1)
		{
			if(++delay_50==10)	 delay_50=0,delay_flag=0;  		//给主函数提供50ms的精准延时，示波器需要50ms高精度延时
		}
		Position_Left+=(-Encoder_Left); 
		Position_Right+=(-Encoder_Right); 
		dusu+=(int)Gyro_Turn/80;
		Key();                                    					//扫描按键状态 单击双击可以改变小车运行状态	
     //xunji_pid();	
		Yaw=Yaw+26;	
      if(Flag_Stop==0)
			{
         
			
				//Turn_Pwm=Turn_ways(xunji_error);
					Turn_Pwm=Turn_ways(Yaw);	
				Left_speed=speed_control_left(Encoder_Left,10);
	      right_speed=speed_control_right(Encoder_Right,10);	
//					Left_speed=Position_control_left(Position_Left,-1056*2);
//					right_speed=Position_control_right(Position_Right,1056*2);	
				Motor_Left=Left_speed+Turn_Pwm;                  //计算左轮电机最终PWM
	      Motor_Right=right_speed-Turn_Pwm;	
		   Set_Pwm(Motor_Left,Motor_Right);         					//赋值给PWM寄存器  
	
				
//				if(0) 
//				{
////					turn_ways=2;
////					turnleft_90=1;

//				Turn_Pwm=Turn_ways(dusu);
//				Motor_Left=Turn_Pwm;                  //计算左轮电机最终PWM
//	   Motor_Right=-Turn_Pwm;
//		   Set_Pwm(Motor_Left,Motor_Right);  
//				}	
//				
				
//			 Motor_Left=Turn_Pwm;                  //计算左轮电机最终PWM
//		   Motor_Right=-Turn_Pwm;	
			}
			if(Flag_Stop==1) Set_Pwm(0,0);         					//赋值给PWM寄存器  
			
			
	 }       	
	 return 0;	  
} 




int speed_control_left(int encoder,int setspeed)
{
   static int pwmout=0,last_error=0,error=0;
	error=setspeed-encoder;
  pwmout=pwmout+pid_speed.kp*(error-last_error)+pid_speed.ki*error;
  last_error=error;
	if(pwmout>6500)
		pwmout=6500;
  if(pwmout<-6500)
		pwmout=-6500;
   return pwmout;
}

int speed_control_right(int encoder,int setspeed)
{
   static int pwmout=0,last_error=0,error=0;
	error=setspeed-encoder;
  pwmout=pwmout+pid_speed.kp*(error-last_error)+pid_speed.ki*error;
  last_error=error;
	if(pwmout>6500)
		pwmout=6500;
  if(pwmout<-6500)
		pwmout=-6500;
   return pwmout;
}

int Position_control_left(int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm=pid_Position.kp*Bias+pid_Position.ki*Integral_bias+pid_Position.kd*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return (int)Pwm;                                           //增量输出
}

int Position_control_right(int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 if(Integral_bias>100000)Integral_bias=10000;
	 if(Integral_bias<-100000)Integral_bias=-10000;
	 Pwm=pid_Position.kp*Bias+pid_Position.ki*Integral_bias+pid_Position.kd*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return (int)Pwm;                                           //增量输出
}





int Turn_ways(int yaw)
{
   static int turn_pwm,bias,last_bias,Turn_Target=1;
	 int Kd;		
//	if(turn_ways==1)
//	{
//		turn_pwm=Turn_Target*yaw*pid_xunji_turn.kp;//结合Z轴陀螺仪进行PD控制
//	}
//	if(turn_ways==2&&turnleft_90==1)
//	{
//		turn_pwm=Turn_Target*(yaw-3700)*pid_Yaw_turn.kp+pid_Yaw_turn.kd;//结合Z轴陀螺仪进行PD控制
//	}
//	if(turn_ways==2&&turnright_90==1)
//	{
//		turn_pwm=Turn_Target*(yaw+3700)*pid_Yaw_turn.kp+pid_Yaw_turn.kd;//结合Z轴陀螺仪进行PD控制
//	}
//	if(turn_ways==2&&turnleft_180==1)
//	{
//		turn_pwm=Turn_Target*(yaw-7000)*pid_Yaw_turn.kp+pid_Yaw_turn.kd;//结合Z轴陀螺仪进行PD控制
//	}
//	bias=yaw-3700*0;
//	turn_pwm=Turn_Target*bias*pid_Yaw_turn.kp+pid_Yaw_turn.kd*(bias-last_bias);//结合Z轴陀螺仪进行PD控制
//	last_bias=bias;
//	 return (int)turn_pwm;								

    bias=yaw-0;
	turn_pwm=Turn_Target*bias*pid_Yaw.kp+pid_Yaw.kd*(bias-last_bias);//结合Z轴陀螺仪进行PD控制
	last_bias=bias;
	 return (int)turn_pwm;	
}

/**************************************************************************
Function: Assign to PWM register
Input   : motor_left：Left wheel PWM；motor_right：Right wheel PWM
Output  : none
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int motor_left,int motor_right)
{
  if(motor_left>0)	    BIN1=1,			BIN2=0; //前进 
	else           			  BIN1=0,			BIN2=1; //后退
	PWMB=myabs(motor_left);	
  if(motor_right>0)			AIN2=1,			AIN1=0;	//前进
	else 	        			  AIN2=0,			AIN1=1; //后退
	PWMA=myabs(motor_right);
}

/**************************************************************************
Function: Press the key to modify the car running state
Input   : none
Output  : none
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
	u8 tmp,tmp2;
	tmp=click_N_Double(50); 
	if(tmp==1)
	{ 
		Flag_Stop=!Flag_Stop;
	}		//单击控制小车的启停
	tmp2=Long_Press();                   
  if(tmp2==1) Flag_Show=!Flag_Show;	//长按控制进入上位机模式，小车的显示停止

}
/**************************************************************************
Function: If abnormal, turn off the motor
Input   : angle：Car inclination；voltage：Voltage
Output  : 1：abnormal；0：normal
函数功能：异常关闭电机		
入口参数：angle：小车倾角；voltage：电压
返回  值：1：异常  0：正常
**************************************************************************/	
u8 Turn_Off(float angle, int voltage)
{
	u8 temp;
	if(1==Flag_Stop||voltage<1110)//电池电压低于11.1V关闭电机
	{	                                                 //倾角大于40度关闭电机
		temp=1;                                          //Flag_Stop置1，即单击控制关闭电机
		AIN1=0;                                            
		AIN2=0;
		BIN1=0;
		BIN2=0;
	}
	else
		temp=0;
	return temp;			
}
	
/**************************************************************************
Function: Get angle
Input   : way：The algorithm of getting angle 1：DMP  2：kalman  3：Complementary filtering
Output  : none
函数功能：获取角度	
入口参数：way：获取角度的算法 1：DMP  2：卡尔曼 3：互补滤波
返回  值：无
**************************************************************************/	
void Get_Angle(u8 way)
{ 
	float Accel_Y,Accel_Z,Accel_X,Accel_Angle_x,Accel_Angle_y,Gyro_X,Gyro_Z,Gyro_Y;
	Temperature=Read_Temperature();      //读取MPU6050内置温度传感器数据，近似表示主板温度。
	if(way==1)                           //DMP的读取在数据采集中断读取，严格遵循时序要求
	{	
		Read_DMP();                      	 //读取加速度、角速度、倾角
		Angle_Balance=Pitch;             	 //更新平衡倾角,前倾为正，后倾为负
		Gyro_Balance=gyro[0];              //更新平衡角速度,前倾为正，后倾为负
		Gyro_Turn=gyro[2];                 //更新转向角速度
		Acceleration_Z=accel[2];           //更新Z轴加速度计
	}			
	else
	{
		Gyro_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_L);    //读取X轴陀螺仪
		Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //读取Y轴陀螺仪
		Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //读取Z轴陀螺仪
		Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //读取X轴加速度计
		Accel_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_L); //读取X轴加速度计
		Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //读取Z轴加速度计
		if(Gyro_X>32768)  Gyro_X-=65536;                 //数据类型转换  也可通过short强制类型转换
		if(Gyro_Y>32768)  Gyro_Y-=65536;                 //数据类型转换  也可通过short强制类型转换
		if(Gyro_Z>32768)  Gyro_Z-=65536;                 //数据类型转换
		if(Accel_X>32768) Accel_X-=65536;                //数据类型转换
		if(Accel_Y>32768) Accel_Y-=65536;                //数据类型转换
		if(Accel_Z>32768) Accel_Z-=65536;                //数据类型转换
		Gyro_Balance=-Gyro_X;                            //更新平衡角速度
		Accel_Angle_x=atan2(Accel_Y,Accel_Z)*180/PI;     //计算倾角，转换单位为度	
		Accel_Angle_y=atan2(Accel_X,Accel_Z)*180/PI;     //计算倾角，转换单位为度
		Gyro_X=Gyro_X/16.4;                              //陀螺仪量程转换，量程±2000°/s对应灵敏度16.4，可查手册
		Gyro_Y=Gyro_Y/16.4;                              //陀螺仪量程转换	
		if(Way_Angle==2)		  	
		{
			 Pitch = -Kalman_Filter_x(Accel_Angle_x,Gyro_X);//卡尔曼滤波
			 Roll = -Kalman_Filter_y(Accel_Angle_y,Gyro_Y);
		}
		else if(Way_Angle==3) 
		{  
			 Pitch = -Complementary_Filter_x(Accel_Angle_x,Gyro_X);//互补滤波
			 Roll = -Complementary_Filter_y(Accel_Angle_y,Gyro_Y);
		}
		Angle_Balance=Pitch;                              //更新平衡倾角
		Gyro_Turn=Gyro_Z;                                 //更新转向角速度
		Acceleration_Z=Accel_Z;                           //更新Z轴加速度计	
	}
}
/**************************************************************************
Function: Absolute value function
Input   : a：Number to be converted
Output  : unsigned int
函数功能：绝对值函数
入口参数：a：需要计算绝对值的数
返回  值：无符号整型
**************************************************************************/	
int myabs(int a)
{ 		   
	int temp;
	if(a<0)  temp=-a;  
	else temp=a;
	return temp;
}
/**************************************************************************
Function: Check whether the car is picked up
Input   : Acceleration：Z-axis acceleration；Angle：The angle of balance；encoder_left：Left encoder count；encoder_right：Right encoder count
Output  : 1：picked up  0：No action
函数功能：检测小车是否被拿起
入口参数：Acceleration：z轴加速度；Angle：平衡的角度；encoder_left：左编码器计数；encoder_right：右编码器计数
返回  值：1:小车被拿起  0：小车未被拿起
**************************************************************************/
int Pick_Up(float Acceleration,float Angle,int encoder_left,int encoder_right)
{ 		   
	 static u16 flag,count0,count1,count2;
	 if(flag==0)                                                      //第一步
	 {
			if(myabs(encoder_left)+myabs(encoder_right)<30)               //条件1，小车接近静止
			count0++;
			else 
			count0=0;		
			if(count0>10)				
			flag=1,count0=0; 
	 } 
	 if(flag==1)                                                      //进入第二步
	 {
			if(++count1>200)       count1=0,flag=0;                       //超时不再等待2000ms，返回第一步
			if(Acceleration>26000&&(Angle>(-20+Middle_angle))&&(Angle<(20+Middle_angle)))   //条件2，小车是在0度附近被拿起
			flag=2; 
	 } 
	 if(flag==2)                                                       //第三步
	 {
		  if(++count2>100)       count2=0,flag=0;                        //超时不再等待1000ms
	    if(myabs(encoder_left+encoder_right)>70)                       //条件3，小车的轮胎因为正反馈达到最大的转速   
      {
				flag=0;                                                                                     
				return 1;                                                    //检测到小车被拿起
			}
	 }
	return 0;
}
/**************************************************************************
Function: Check whether the car is lowered
Input   : The angle of balance；Left encoder count；Right encoder count
Output  : 1：put down  0：No action
函数功能：检测小车是否被放下
入口参数：平衡角度；左编码器读数；右编码器读数
返回  值：1：小车放下   0：小车未放下
**************************************************************************/
int Put_Down(float Angle,int encoder_left,int encoder_right)
{ 		   
	 static u16 flag,count;	 
	 if(Flag_Stop==0)                     //防止误检      
			return 0;	                 
	 if(flag==0)                                               
	 {
			if(Angle>(-10+Middle_angle)&&Angle<(10+Middle_angle)&&encoder_left==0&&encoder_right==0) //条件1，小车是在0度附近的
			flag=1; 
	 } 
	 if(flag==1)                                               
	 {
		  if(++count>50)                     //超时不再等待 500ms
		  {
				count=0;flag=0;
		  }
	    if(encoder_left>3&&encoder_right>3&&encoder_left<40&&encoder_right<40) //条件2，小车的轮胎在未上电的时候被人为转动  
      {
				flag=0;
				flag=0;
				return 1;                         //检测到小车被放下
			}
	 }
	return 0;
}

/**************************************************************************
Function: Select car running mode
Input   : encoder_left：Left wheel encoder reading；encoder_right：Right wheel encoder reading
Output  : none
函数功能：选择小车运行模式
入口参数：encoder_left：左编码器读数  encoder_right：右编码器读数
返回  值：无
**************************************************************************/
void Choose(int encoder_left,int encoder_right)
{
	static int count;
	if(Flag_Stop==0)
		count = 0;
	if((Flag_Stop==1)&&(encoder_left<10))	//此时停止且左轮不动
	{
		count += myabs(encoder_right);
		if(count>6&&count<180)	//普通模式
		{
			Flag_follow = 0;
			Flag_avoid = 0;
		}
		if(count>180&&count<360)	//避障模式 
		{
			Flag_avoid = 1;
			Flag_follow = 0;
		}
		if(count>360&&count<540)	//跟随模式
		{
			Flag_avoid = 0;
			Flag_follow = 1;
		}
		if(count>540)
			count = 0;
	}
}

void xunji_pid(void)
{

	static int i;
	if(xunji)
	{
		
									if(Left==0&&Right==0)		
									{
										xunji_error=0;
										turn_ways=1;
									
									}	 
	
									if(Left==1&&Right==0)	   
									{
									 xunji_error=40;
										turn_ways=1;
									}			
	
								 if(Left==0&&Right==1)	
								 {
									xunji_error=-40;
									 turn_ways=1;
								 
								 }		

								 if(Left==1&&Right==1&&medicine==0)
								 {
									dusu=0;
									 xunji=0;
									 turn=1;
									 turnleft90_flag=1;
								 
								 }		
		

								if(Left==1&&Right==1&&medicine==1)	
								{
								
									 dusu=0;
									 xunji=0;
									 turn=1;
									 turnright90_flag=1;
								}
//	
//	if(stop==0) 
//	{
//	stop_cnt++;
//	if(stop_cnt>50)
//	{
//	Flag_Stop=1;	
//	dusu=0;	
//		if(medicine==1)
//		{
//				xunji=0;
//			turn=1; 
//			turnleft180_flag=1;
//			Flag_Stop=0;	
////		stopflag=0;	
//	stop_cnt=0;
//		
//		}	
//	}

//	}	


	
	
	
	
	}

	
	
	
	
	//转向模式
	
	if(turn)
	{	
								i++;
								turn_ways=2;
								if(turnleft90_flag==1) turnleft_90=1;
								if(turnright90_flag==1) turnright_90=1;
								if(turnleft180_flag==1) turnleft_180=1;
	if(i>400)//转向完成
	{
	  turn=0;
		turnleft90_flag=0;
		turnright90_flag=0;
		turnleft180_flag=0;
		dusu=0;
		xunji=1;
		i=0;
	}	
	}
	 
}

