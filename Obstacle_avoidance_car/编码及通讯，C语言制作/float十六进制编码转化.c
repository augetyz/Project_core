#include <stdio.h>
void test_float_to_4hex(float num,unsigned short* add)
{
	unsigned char tbuf[4];
	unsigned char *p = (unsigned char*)&num + 3;//ָ��p��ָ��float������ֽ�

	//��ȡ��Ӧ��4���ֽڣ��ӵ�λ����λ����ʱ�Ϳ�������hex��ʽ�����ݴ�����
	tbuf[0] = *(p-3);
	tbuf[1] = *(p-2);
	tbuf[2] = *(p-1);
	tbuf[3] = *p;
	*add=*tbuf;*(add+1)=*(tbuf+1);*(add+2)=*(tbuf+2);*(add+3)=*(tbuf+3);
}
float test_4hex_to_float(unsigned short* add)
{
	float res;//��֤float���Ϊ4���ֽں�����Ϊfloat�Ľ��

	//�Բ�ֺ��4���ֽڽ������飬ģ����յ�hex������ݻ�ԭ����
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
