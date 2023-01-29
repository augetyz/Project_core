#ifndef __IIC_H
#define __IIC_H
#include "Sys.h"


   	   		   
//IO方向设置
#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //输入SDA 

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void); 
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_Send_Byte(u8 txd);
void IIC_Write_Addr_Data(u8 address,u8 data);
void IIC_NAck(void);
u8 IIC_Read_Byte(void);
u8 IIC_Read_Addr_Data(u8 address);
 
#endif