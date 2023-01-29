#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "math.h"
#include "string.h"	
#include "gps.h"
#include "usart2.h"
#include "stdio.h"
nmea_msg gpsx; 
u8 dtbuf[50];
u8 USART1_TX_BUF[USART2_MAX_RECV_LEN];
const u8*fixmode_tbl[4]={"Fail","Fail"," 2D "," 3D "};
void Gps_Msg_Show(void);
int main(void)
{	
	u16 i,rxlen;
	u8 upload=0;
	delay_init();	
	LED_Init();//LED初始化
	LCD_Init();//LCD初始化
	USART2_Init(9600);
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	LED=0;
	while(1) 
	{		
		delay_ms(1);
		if(USART2_RX_STA&0X8000)		//接收到一次数据了
		{
			rxlen=USART2_RX_STA&0X7FFF;	//得到数据长度
			for(i=0;i<rxlen;i++)USART1_TX_BUF[i]=USART2_RX_BUF[i];	   
 			USART2_RX_STA=0;		   	//启动下一次接收
			USART1_TX_BUF[i]=0;			//自动添加结束符
			GPS_Analysis(&gpsx,(u8*)USART1_TX_BUF);//分析字符串
			Gps_Msg_Show();				//显示信息	
			if(upload)
				printf("\r\n%s\r\n",USART1_TX_BUF);//发送接收到的数据到串口1
 		}
	}
}
void Gps_Msg_Show(void)
{
 	float tp;		     	 
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf,"Longitude:%.5f %1c   ",tp/=100000,gpsx.ewhemi);	//得到经度字符串
 	LCD_ShowString(0,16-16,dtbuf,RED,WHITE,16,0);	   
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf,"Latitude:%.5f %1c   ",tp/=100000,gpsx.nshemi);	//得到纬度字符串
 	LCD_ShowString(0,32-16,dtbuf,RED,WHITE,16,0);	 	 
//	tp=gpsx.altitude;	   
// 	sprintf((char *)dtbuf,"Altitude:%.1fm     ",tp/=10);	    			//得到高度字符串
// 	LCD_ShowString(0,48,dtbuf,RED,WHITE,16,0);			   
//	tp=gpsx.speed;	   
// 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    		//得到速度字符串	 
// 	LCD_ShowString(0,64,dtbuf,RED,WHITE,16,0); 				    
	if(gpsx.fixmode<=3)														//定位状态
	{  
	sprintf((char *)dtbuf,"Fix Mode:%s",fixmode_tbl[gpsx.fixmode]);	
	LCD_ShowString(0,48-16,dtbuf,RED,WHITE,16,0);	   
	}	 	   
	sprintf((char *)dtbuf,"GPSuse :%02d",gpsx.posslnum);	 		//用于定位的GPS卫星数
 	LCD_ShowString(0,64-16,dtbuf,RED,WHITE,16,0);  
	sprintf((char *)dtbuf,"GPSsee:%02d",gpsx.svnum%100);	 		//可见GPS卫星数
 	LCD_ShowString(0,80-16,dtbuf,RED,WHITE,16,0);
	
//	sprintf((char *)dtbuf,"BD Visible satellite:%02d",gpsx.beidou_svnum%100);	 		//可见北斗卫星数
// 	LCD_ShowString(0,96,dtbuf,RED,WHITE,16,0);
	
	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//显示UTC日期
	LCD_ShowString(0,96-16,dtbuf,RED,WHITE,16,0);	    
	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//显示UTC时间
	LCD_ShowString(0,112-16,dtbuf,RED,WHITE,16,0);	  
}
