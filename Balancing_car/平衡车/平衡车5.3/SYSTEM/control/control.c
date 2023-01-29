#include "control.h"

extern float Pitch,Roll,Yaw;
extern short gyro[3], accel[3], sensors;
float Med_Angle=-0.6;      // ��е��ֵ����ʹ��С������ƽ��ס�ĽǶ� 
float Target_Speed=0;	  // �����ٶȡ�---���ο����ӿڣ����ڿ���С��ǰ�����˼����ٶȡ�
float 
  Vertical_Kp=310,
  Vertical_Kd=1.0;     // ֱ����Kp��Kd
float 
  Velocity_Kp=0,     // �ٶȻ�Kp��Ki����������
  Velocity_Ki=0;
float 
  Turn_Kp=0;

int Vertical_out,Velocity_out,Turn_out; // ֱ����&�ٶȻ�&ת�򻷵��������

int Vertical(float Med,float Angle,float gyro_Y); // ��������
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z);

/*void EXTI9_5_IRQHandler(void)
{
  int PWM_out;
  if(EXTI_GetITStatus(EXTI_Line5)!=0) // һ���ж�
  {
    if(PBin(5)==0)    // �����ж�
    { 
      EXTI_ClearITPendingBit(EXTI_Line5); // ����жϱ�־λ
      // 1.�ɼ�����������&MPU6050�Ƕ���Ϣ
      // �������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ��
      Encoder_Left  = -Read_Speed(2); 
      Encoder_Right = Read_Speed(4);
      
      mpu_dmp_get_data(&Pitch,&Roll,&Yaw);	    // ��ȡ�Ƕ�
      MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);  // ��ȡ���ٶ�
      MPU_Get_Accelerometer(&aacx,&aacy,&aacz); // ��ȡ���ٶ�
      // 2.������ѹ��ջ������У���������������
	  Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right); // �ٶȻ�
      Vertical_out=Vertical(Velocity_out+Med_Angle,Roll,gyrox);			  // ֱ����
			Turn_out=Turn(gyroz);	
      
      PWM_out=Vertical_out;//�������
      
      // 3.�ѿ�����������ص�����ϣ�������տ���
      MOTO1 = PWM_out-Turn_out; // ����
      MOTO2 = PWM_out+Turn_out; // �ҵ��
      Limit(&MOTO1,&MOTO2);     // PWM�޷�
      Load(MOTO1,MOTO2);        // ���ص������
    }
  }
}
*/
/*****************  
ֱ����PD��������Kp*Ek+Kd*Ek_D

��ڣ�Med:��е��ֵ(�����Ƕ�)��Angle:��ʵ�Ƕȣ�gyro_Y:��ʵ���ٶ�
���ڣ�ֱ�������
******************/
int Vertical(float Med,float Angle,float gyro_Y) 
{
  int PWM_out;
  
  PWM_out = Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
  
  return PWM_out;
} 

/*****************  
�ٶȻ�PI��������Kp*Ek+Ki*Ek_S(Ek_S��ƫ��Ļ���)
******************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
  // ����ɾ�̬�����������ھ�̬�洢����ʹ�ñ���������
  static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;
  float a=0.7;
  
  // 1.�����ٶ�ƫ��
  //��ȥ���--�ҵ���⣺�ܹ����ٶ�Ϊ"0"�ĽǶȣ����ǻ�е��ֵ��
  Encoder_Err = ((encoder_left+encoder_right)-Target);
  // 2.���ٶ�ƫ����е�ͨ�˲�
  // low_out = (1-a)*Ek+a*low_out_last
  EnC_Err_Lowout = (1-a)*Encoder_Err + a*EnC_Err_Lowout_last; // ʹ�ò��θ���ƽ�����˳���Ƶ���ţ������ٶ�ͻ��
  EnC_Err_Lowout_last = EnC_Err_Lowout;   // ��ֹ�ٶȹ���Ӱ��ֱ��������������
  // 3.���ٶ�ƫ����ֳ�λ��
  Encoder_S+=EnC_Err_Lowout;
  // 4.�����޷�
  Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
  
  // 5.�ٶȻ��������
  PWM_out = Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
  
  return PWM_out;
}

/*****************  
ת�򻷣�ϵ��*Z����ٶ�
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
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
		Read_DMP(&Pitch,&Roll,&Yaw); TOGGLE1;
		Vertical_out=Vertical(Med_Angle,Pitch,gyro[1]);	
		PWM_out=Vertical_out;//�������
		motor(PWM_out,PWM_out);
	}	
}

