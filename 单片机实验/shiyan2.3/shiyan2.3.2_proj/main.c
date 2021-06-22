#include<reg51.h>
#include<stdio.h>
char data a[32] _at_ 0x30;
void main()
{
	char
//定义数组
table[32]={1,3,9,2,17,4,11,6,5,20,100,64,21,14,79,35,92,23,65,16,13,18,18,73,65,101,27,19,62,69};
	char i,j,temp;
	for(i=0;i<32;i++)
{a[i]=table[i];}
//两层循环 冒泡排序法
for(i=0;i<31;i++)
{
	for(j=0;j<31-i;j++)
	{
	if(a[j]>a[j+1])
	{
	  temp=a[j];
	  a[j]=a[j+1];
	  a[j+1]=temp;
	  }
	  }
	  }
while(1);
}
