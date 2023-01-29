#include<stdio.h>
#include <string.h>
typedef char* string;
string get_string(char x,char y,string target)
{
	string biu;
	char biu1[1024];
	int a=0,b=0,c=0,d;
	while(*(target+a++)!=x);
	//a--;		
	c=strlen(target);
	printf("%d\n",c);
	while(*(target+c-b++)!=y);
	b--;
	printf("%d\n",b);
	for(d=0;d<(c-b-a);d++)
	{
		biu1[d]=*(target+a+d);
	}
	biu1[d]='\0';
	biu=biu1;
	return biu;
}
main()
{	
	string a;string b;
	a="method:thing.service.property.set,id:1789519928,params:{Status:1";
	b=&a;
	printf("%s\n",a);
	printf("%s\n",get_string('a','h',get_string('{','[',a)));
	
 } 
