#include "stm32f10x.h"
#include "sys.h"
//////////////////////////ȫ�ֱ����Ķ���////////////////////////////////////  
float pitch,roll,yaw; 								  			 //ŷ����(��̬��)
short aacx,aacy,aacz;													 //���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;											 //������ԭʼ����
float temp; 								  								 //�¶�
int main(void)	
{ 
	delay_init();	    	           //=====��ʱ������ʼ��	
	NVIC_Configuration();					 //=====�ж����ȼ�����,���а��������е��ж����ȼ�������,��������һ�����޸ġ�
	uart1_init(115200);	             //=====����1��ʼ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	MPU_Init();					    			 //=====��ʼ��MPU6050
	mpu_dmp_init();								 //=====��ʼ��MPU6050��DMPģʽ	
  while(1)	
	{
		/*�������ǲ鿴���ݵı仯,�鿴��̬��;�����Ҫ�����ٶ��Լ����ٶȿ������޸�***/
		mpu_dmp_get_data(&pitch,&roll,&yaw);			//�õ���̬�Ǽ�ŷ����
		temp=MPU_Get_Temperature();								//�õ��¶�ֵ
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������	
			
		printf("pitch=%.2f,roll=%.2f,yaw=%.2f,temp=%.2f\n",pitch,roll,yaw,temp/100);
		/*Ҳ����ʹ�õ�����λ���鿴,���û�й���OLED��Ļ�Ŀͻ��򿪵�����λ��,�����������ó�9600��ʹ�ô��ڴ�ӡ����λ�����ɲ鿴***/
	} 	
}

