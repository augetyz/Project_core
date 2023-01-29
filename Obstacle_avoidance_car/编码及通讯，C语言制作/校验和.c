#include<stdio.h>
#define u8 unsigned char 
u8 check_sum1(u8 *addr)//Ð£ÑéºÍ
{
  u8 sum=0;int biu1;
  
  for(biu1=0;biu1<24;biu1++)
  {
  	sum+=addr[biu1];
  }
  if(sum>0xff)
  	sum=sum&0xff;
  return sum;
}
unsigned short check_sum(unsigned short *addr)
{
  int biu;int sum=0;
  int* biubiu=addr;
  for(biu=0;biu<24;biu++)
  {
  	printf("%x+%x=",sum,biubiu[biu]);
  	printf("%x\n",sum=sum+biubiu[biu]);
  	if(sum>0xff)
  	sum=sum&0xff;
  //	printf("%x\n",sum);
  }

  if(sum>0xff)
  	sum=sum&0xff;
  return sum;
}

u8 check_sum2(u8 *addr)
{
  int biu;int sum=0;
  u8* biubiu=addr;
  for(biu=0;biu<24;biu++)
  {
  	
  	sum=sum+biubiu[biu];
  	if(sum>0xff)
  	sum=sum&0xff;
  	//printf("%x\n",sum);
  }
  if(sum>0xff)
  sum=sum&0xff;
  return sum;
}
main()
{
	u8 date[24]={0X53,0X5A,0X48,0X59,0X01,0X17,0X00,0X00,0X00,0X10,0XE9,0X26,0X11,0X3F,0XE9,0X26,0X11,0X3F,0XE9,0X26,0X11,0X3F};
	//int date2[24]={0X53,0X5A,0X48,0X59,0X01,0X17,0X00,0X00,0X00,0X10,0XE9,0X26,0X11,0X3F,0XE9,0X26,0X11,0X3F,0XE9,0X26,0X11,0X3F};


	printf("%x\n",check_sum2(date));
	printf("%x\n",check_sum(date));

}













