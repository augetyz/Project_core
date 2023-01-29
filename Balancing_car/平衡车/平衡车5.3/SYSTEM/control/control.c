#include "control.h"

extern float Pitch,Roll,Yaw;
extern short gyro[3], accel[3], sensors;
float Med_Angle=-0.6;      // 机械中值，能使得小车真正平衡住的角度 
float Target_Speed=0;	  // 期望速度。---二次开发接口，用于控制小车前进后退及其速度。
float 
  Vertical_Kp=310,
  Vertical_Kd=1.0;     // 直立环Kp、Kd
float 
  Velocity_Kp=0,     // 速度环Kp、Ki（正反馈）
  Velocity_Ki=0;
float 
  Turn_Kp=0;

int Vertical_out,Velocity_out,Turn_out; // 直立环&速度环&转向环的输出变量

int Vertical(float Med,float Angle,float gyro_Y); // 函数声明
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z);

/*void EXTI9_5_IRQHandler(void)
{
  int PWM_out;
  if(EXTI_GetITStatus(EXTI_Line5)!=0) // 一级判定
  {
    if(PBin(5)==0)    // 二级判断
    { 
      EXTI_ClearITPendingBit(EXTI_Line5); // 清除中断标志位
      // 1.采集编码器数据&MPU6050角度信息
      // 电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反
      Encoder_Left  = -Read_Speed(2); 
      Encoder_Right = Read_Speed(4);
      
      mpu_dmp_get_data(&Pitch,&Roll,&Yaw);	    // 读取角度
      MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);  // 读取角速度
      MPU_Get_Accelerometer(&aacx,&aacy,&aacz); // 读取加速度
      // 2.将数据压入闭环控制中，计算出控制输出量
	  Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right); // 速度环
      Vertical_out=Vertical(Velocity_out+Med_Angle,Roll,gyrox);			  // 直立环
			Turn_out=Turn(gyroz);	
      
      PWM_out=Vertical_out;//最终输出
      
      // 3.把控制输出量加载到电机上，完成最终控制
      MOTO1 = PWM_out-Turn_out; // 左电机
      MOTO2 = PWM_out+Turn_out; // 右电机
      Limit(&MOTO1,&MOTO2);     // PWM限幅
      Load(MOTO1,MOTO2);        // 加载到电机上
    }
  }
}
*/
/*****************  
直立环PD控制器：Kp*Ek+Kd*Ek_D

入口：Med:机械中值(期望角度)，Angle:真实角度，gyro_Y:真实角速度
出口：直立环输出
******************/
int Vertical(float Med,float Angle,float gyro_Y) 
{
  int PWM_out;
  
  PWM_out = Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
  
  return PWM_out;
} 

/*****************  
速度环PI控制器：Kp*Ek+Ki*Ek_S(Ek_S：偏差的积分)
******************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
  // 定义成静态变量，保存在静态存储器，使得变量不丢掉
  static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;
  float a=0.7;
  
  // 1.计算速度偏差
  //舍去误差--我的理解：能够让速度为"0"的角度，就是机械中值。
  Encoder_Err = ((encoder_left+encoder_right)-Target);
  // 2.对速度偏差进行低通滤波
  // low_out = (1-a)*Ek+a*low_out_last
  EnC_Err_Lowout = (1-a)*Encoder_Err + a*EnC_Err_Lowout_last; // 使得波形更加平滑，滤除高频干扰，放置速度突变
  EnC_Err_Lowout_last = EnC_Err_Lowout;   // 防止速度过大影响直立环的正常工作
  // 3.对速度偏差积分出位移
  Encoder_S+=EnC_Err_Lowout;
  // 4.积分限幅
  Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
  
  // 5.速度环控制输出
  PWM_out = Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
  
  return PWM_out;
}

/*****************  
转向环：系数*Z轴角速度
******************/
int Turn(int gyro_Z)
{
  int PWM_out;
  
  PWM_out = Turn_Kp*gyro_Z;
  
  return PWM_out;
}

void TIM4_IRQHandler(void)
{
	int PWM_out;
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
		Read_DMP(&Pitch,&Roll,&Yaw); TOGGLE1;
		Vertical_out=Vertical(Med_Angle,Pitch,gyro[1]);	
		PWM_out=Vertical_out;//最终输出
		motor(PWM_out,PWM_out);
	}	
}

