#include<reg51.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
uchar data a[32] _at_ 0x30; //�趨����a ����ʼ��ַΪ30H
uint i _at_ 0x55; //������i ���ڵ�ַ55H
//��ʱ����
void DelayMS(uint ms)
{
uchar t;
while(ms--) for(t=0;t<120;t++);
}
//������
void main()
{
SP=0x60; //�趨��ջָ��λ��
SCON=0x52;
TMOD=0x20;
TH1=0xf3;
TR1=1; //���м�����3 ��ΪPRINTF ����������
for(i=0;i<32;i++)
{
a[i]=i;
printf("It is : %d now.\n",i); //��ӡ����ִ�е���Ϣ
DelayMS(20000);
}
while(1);
}