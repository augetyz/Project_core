#include<stdio.h>
#include<math.h>

#define u8 unsigned char
#define uint8_t unsigned int  

float p,i,d;

float pid_date(u8* a)
{
	int l,m,n,b;float k=0;
	for(l=0;a[l]!=':';l++);//1
	for(m=0;a[m]!='.';m++)//4
	{
		if(a[m]=='\n')
		break;
	}
	for(n=0;a[n]!='\n';n++);//8
	for(l++;l<m;l++)
	{		
		k+=(a[l]-0x30)*pow(10,(double)(m-l-1));
	}
	for(b=m++;m<n;m++)
	{		
		k+=(a[m]-0x30)*pow(0.1,(double)(m-b));
	}
	return k;
}

main()
{
	u8 date[20]="i:2.789\n";
	//0 1 2 3 4 5 6 7  8 
	//i : 2 1 . 3 2 1 \n
	int num;
	for(num=0;date[num]!='\n';num++)
	{
		printf("%x ",date[num]);
	}
	printf("%.3f %.3f %.3f\n",p,i,d);	
	p=pid_date(date);
	i=pid_date(date);
	d=pid_date(date);
	printf("%.3f %.3f %f\n",p,i,d);	

 } 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
