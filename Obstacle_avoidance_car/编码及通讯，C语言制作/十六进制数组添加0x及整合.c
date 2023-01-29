#include<stdio.h>
int main()
{
	char c[300];
	int i=0;
	while(1)	
{
	for(i=0;(c[i]=getchar())!='\n';i++)
	{
		if(i==0)
		printf("{"); 
		if(c[i]==' ')
		i--;
		else
		{
		if((i%2)==0)
		printf("0X");
		printf("%c",c[i]);
		if((i%2)!=0)
		printf(",");
		}
	}
	i=(i+1)/2;
	for(;i<60;i++)
	printf("0,");
	printf("}");
	printf(",\n\n");
}
	return 0;
 } 
