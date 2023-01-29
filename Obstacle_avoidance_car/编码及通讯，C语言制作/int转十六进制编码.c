#include<stdio.h>
main()
{
	int a=0XABCDEF15;
	
	printf("%x\n",a&0xff);
	printf("%x\n",a>>8&0xff);
 } 
