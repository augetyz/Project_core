#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "mpu6050.h"
#include "usmart.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
 #include "oled_iic.h"
/*******************
OLED接线定义:
					VCC--3.3V/5V
					GND--GND 
					SCL--PB8
					SDA--PB9
MPU6050接线定义：
					VCC--5V
					GND--GND 
					SCL--PB6
					SDA--PB7
					
					
					2020/12/16 15：10
*************************/

////串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{   	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
	USART_SendData(USART1,c);  
} 
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)//匿名上位机协议
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+4]=0;	//校验数置零
	send_buf[0]=0XAA;	//帧头
	send_buf[1]=0XAA;	//帧头
	send_buf[2]=fun;	//功能字
	send_buf[3]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[4+i]=data[i];			//复制数据
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+5;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}


//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart1_niming_report(0X02,tbuf,12);//飞机传感器数据,0X02
}	


//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
	tbuf[0]=(roll>>8)&0XFF;
	tbuf[1]=roll&0XFF;
	tbuf[2]=(pitch>>8)&0XFF;
	tbuf[3]=pitch&0XFF;
	tbuf[4]=(yaw>>8)&0XFF;
	tbuf[5]=yaw&0XFF;
	usart1_niming_report(0X01,tbuf,28);//飞机姿态等基本信息，0X01
}  

 	
 int main(void)
 {	 
//	u8 report=1;			//默认开启上报
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	u8 t,m;
	 	short temp;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(500000);	 	//串口初始化为500000
	delay_init();	//延时初始化 
	usmart_dev.init(72);		//初始化USMART
	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	MPU_Init();					//初始化MPU6050
	mpu_dmp_init();
  	OLED_Init();		 	//OLED屏幕初始化		
		OLED_Clear();	
	 
	 //显示pitch和小数点
	 OLED_ShowCH(1,0,"pitch");
	 OLED_ShowCH(60+27,0,".");
	 //显示roll和小数点
		OLED_ShowCH(1,2,"roll");
		OLED_ShowCH(60+27,2,".");
	 //显示yaw和小数点
		OLED_ShowCH(1,4,"yaw");
		OLED_ShowCH(60+27,4,".");
	 
	 	 t = mpu_dmp_init() ;
	 			while(	mpu_dmp_init() )//mpu6050自检
		{
		
		OLED_ShowCH(0,0," error");	
		OLED_ShowNum(0,3,t,2,1);	
		}
 	while(1)
	{
 
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			 LED0=0;
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//		  if(report)mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//用自定义帧发送加速度和陀螺仪原始数据
//			if(report)usart1_report_imu((int)-(roll*100),(int)(pitch*100),(int)-(yaw*100));//左右
//			temp=MPU_Get_Temperature();    //获取温度值
			
			 if((m%10)==0)
	 {
			temp = pitch * 10;
			if(temp<0)
			{
				OLED_ShowCH(60-7,0,"-");//显示负号
				temp = -temp;//将temp改为正数
			}
			else OLED_ShowCH(60-7,0," ");//不显示负号
			
				OLED_ShowNum(60+35,0,temp%10,1,16);			//显示小数部
				OLED_ShowNum(60,0,temp/10,3,16);	//显示整数部分	
			
			temp = roll * 10;
			if(temp<0)
			{
				OLED_ShowCH(60-7,2,"-");//显示负号
				temp = -temp;//将temp改为正数
			}
			else OLED_ShowCH(60-7,2," ");//不显示负号
	
				OLED_ShowNum(60+35,2,temp%10,1,16);	//显示小数部
				OLED_ShowNum(60,2,temp/10,3,16);	//显示整数部分
			
			temp = yaw * 10;
			if(temp<0)
			{
				OLED_ShowCH(60-7,4,"-");//显示负号
				temp = -temp;//将temp改为正数
			}
			else OLED_ShowCH(60-7,4," ");//不显示负号
			
				OLED_ShowNum(60+35,4,temp%10,1,16);		//显示小数部
				OLED_ShowNum(60,4,temp/10,3,16);		//显示整数部分	
		} 
	}		
		m++;
	}
}
 


