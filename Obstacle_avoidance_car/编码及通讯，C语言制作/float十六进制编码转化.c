#include <stdio.h>
void test_float_to_4hex(float num,unsigned short* add)
{
	unsigned char tbuf[4];
	unsigned char *p = (unsigned char*)&num + 3;//指针p先指向float的最高字节

	//获取对应的4个字节，从低位到高位，这时就可以用于hex格式的数据传输了
	tbuf[0] = *(p-3);
	tbuf[1] = *(p-2);
	tbuf[2] = *(p-1);
	tbuf[3] = *p;
	*add=*tbuf;*(add+1)=*(tbuf+1);*(add+2)=*(tbuf+2);*(add+3)=*(tbuf+3);
}
float test_4hex_to_float(unsigned short* add)
{
	float res;//验证float拆分为4个字节后，重组为float的结果

	//对拆分后的4个字节进行重组，模拟接收到hex后的数据还原过程
	unsigned char *pp = (unsigned char*)&res;
	pp[0] = add[0];
	pp[1] = add[1];  
	pp[2] = add[2];
	pp[3] = add[3];
	return res;
}
main()
{
	float a=1.01;int i;
	unsigned short biu[4]={0xcd,0xcc,0x4c,0x3D};
	unsigned short biubiu[4];
	test_float_to_4hex(a,biubiu);
	for(i=0;i<4;i++)
	printf("%x ",biubiu[i]);
	printf("\n%.3f",test_4hex_to_float(biu));
} 
