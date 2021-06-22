#include<reg51.h>
#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
uchar data a[32] _at_ 0x30; //设定数组a 的起始地址为30H
uint i _at_ 0x55; //将变量i 放在地址55H
//延时程序
void DelayMS(uint ms)
{
uchar t;
while(ms--) for(t=0;t<120;t++);
}
//主程序
void main()
{
SP=0x60; //设定堆栈指针位置
SCON=0x52;
TMOD=0x20;
TH1=0xf3;
TR1=1; //此行及以上3 行为PRINTF 函数所必须
for(i=0;i<32;i++)
{
a[i]=i;
printf("It is : %d now.\n",i); //打印程序执行的信息
DelayMS(20000);
}
while(1);
}