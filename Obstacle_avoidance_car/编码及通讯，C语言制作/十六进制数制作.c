#include<stdio.h>
int main()
{
	int i;
	for(i=127;i>=0;i--) 
	{
	if(i<16)
	printf("0x0%X,",i);
	else
	printf("0x%X,",i);
	if((i)%16==0&&i!=0)
	printf("\n");
}
	return 0;
 } 
