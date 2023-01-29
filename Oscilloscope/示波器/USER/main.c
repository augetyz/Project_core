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
	LED_Init();//LED��ʼ��
	LCD_Init();//LCD��ʼ��
	USART2_Init(9600);
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	LED=0;
	while(1) 
	{		
		delay_ms(1);
		if(USART2_RX_STA&0X8000)		//���յ�һ��������
		{
			rxlen=USART2_RX_STA&0X7FFF;	//�õ����ݳ���
			for(i=0;i<rxlen;i++)USART1_TX_BUF[i]=USART2_RX_BUF[i];	   
 			USART2_RX_STA=0;		   	//������һ�ν���
			USART1_TX_BUF[i]=0;			//�Զ���ӽ�����
			GPS_Analysis(&gpsx,(u8*)USART1_TX_BUF);//�����ַ���
			Gps_Msg_Show();				//��ʾ��Ϣ	
			if(upload)
				printf("\r\n%s\r\n",USART1_TX_BUF);//���ͽ��յ������ݵ�����1
 		}
	}
}
void Gps_Msg_Show(void)
{
 	float tp;		     	 
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf,"Longitude:%.5f %1c   ",tp/=100000,gpsx.ewhemi);	//�õ������ַ���
 	LCD_ShowString(0,16-16,dtbuf,RED,WHITE,16,0);	   
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf,"Latitude:%.5f %1c   ",tp/=100000,gpsx.nshemi);	//�õ�γ���ַ���
 	LCD_ShowString(0,32-16,dtbuf,RED,WHITE,16,0);	 	 
//	tp=gpsx.altitude;	   
// 	sprintf((char *)dtbuf,"Altitude:%.1fm     ",tp/=10);	    			//�õ��߶��ַ���
// 	LCD_ShowString(0,48,dtbuf,RED,WHITE,16,0);			   
//	tp=gpsx.speed;	   
// 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    		//�õ��ٶ��ַ���	 
// 	LCD_ShowString(0,64,dtbuf,RED,WHITE,16,0); 				    
	if(gpsx.fixmode<=3)														//��λ״̬
	{  
	sprintf((char *)dtbuf,"Fix Mode:%s",fixmode_tbl[gpsx.fixmode]);	
	LCD_ShowString(0,48-16,dtbuf,RED,WHITE,16,0);	   
	}	 	   
	sprintf((char *)dtbuf,"GPSuse :%02d",gpsx.posslnum);	 		//���ڶ�λ��GPS������
 	LCD_ShowString(0,64-16,dtbuf,RED,WHITE,16,0);  
	sprintf((char *)dtbuf,"GPSsee:%02d",gpsx.svnum%100);	 		//�ɼ�GPS������
 	LCD_ShowString(0,80-16,dtbuf,RED,WHITE,16,0);
	
//	sprintf((char *)dtbuf,"BD Visible satellite:%02d",gpsx.beidou_svnum%100);	 		//�ɼ�����������
// 	LCD_ShowString(0,96,dtbuf,RED,WHITE,16,0);
	
	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//��ʾUTC����
	LCD_ShowString(0,96-16,dtbuf,RED,WHITE,16,0);	    
	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//��ʾUTCʱ��
	LCD_ShowString(0,112-16,dtbuf,RED,WHITE,16,0);	  
}
