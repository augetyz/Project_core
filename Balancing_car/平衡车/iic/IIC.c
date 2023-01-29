#include "IIC.h"
#include "Delay.h"


//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ�� 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	IIC_SCL=1;
	IIC_SDA=1;
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	Delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}
	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delay_us(4);
	IIC_SCL=1; 
	Delay_us(4); //hu add
	IIC_SDA=1;//����I2C���߽����ź�
	//Delay_us(4);// hu cut							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;

	//IIC_SDA=0;Delay_us(1);//hu add

	SDA_IN();      //SDA����Ϊ����  
	//IIC_SDA=1;Delay_us(1);//hu cut	   
	IIC_SCL=1;Delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 


//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	Delay_us(2);
	IIC_SCL=1;
	Delay_us(2);
	IIC_SCL=0;

}

//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	Delay_us(2);
	IIC_SCL=1;
	Delay_us(2);
	IIC_SCL=0;
}

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		Delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		Delay_us(2); 
		IIC_SCL=0;	
		Delay_us(2);
    }	 
}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(void)
{
	u8 i,receive=0x00;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        Delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)
			receive|= 0x01;   
		Delay_us(1); 
    }					 
    return receive;
}


//IIC ��ָ����ַAddress��дData
void IIC_Write_Addr_Data(u8 address,u8 data)
{
	u8 w_addr,w_data;
	w_addr = address;
	w_data = data;
	
	IIC_Start();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	IIC_Send_Byte(w_addr);
	IIC_Ack();
	IIC_Send_Byte(w_data);
	IIC_Ack();
	IIC_Stop();
	Delay_us(4);
}

//IIC ��ָ����ַAddress��дData
u8 IIC_Read_Addr_Data(u8 address)
{
	u8 r_addr,data=0x00;
	r_addr = address;;
	
	IIC_Start();
	IIC_Send_Byte(0x00);
	IIC_Wait_Ack();
	IIC_Send_Byte(r_addr);
	IIC_Ack();

	IIC_Start();
	IIC_Send_Byte(0x01);


	IIC_Wait_Ack();
	data = IIC_Read_Byte();
	IIC_NAck();
	IIC_Stop();
	Delay_us(4);
	return data;
}
