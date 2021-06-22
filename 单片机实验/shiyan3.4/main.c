#include <reg51.h>
#include <stdio.h>
#include <intrins.h>
#define unit unsigned int
#define uchar unsigned char

//这个是C51的关键字，进行外部声明的
sbit D5=P0^0;
sbit D4=P0^1;
sbit D3=P0^2;
sbit D2=P0^3;
sbit D1=P0^4;

uchar data key _at_ 0x30;

uchar code KEY_TABLE[]={0x22,0x12,0x21,0x11};
uchar code OUT_TABLE[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
//这里是延迟函数的编写
void delay(uchar n)
	{
	 uchar t,i;
	 uchar m=200;
	 while(m--)
	 for(i=0;i<n;i++)
	 {
	 for(t=0;t<120;t++);
	 }
	}
	

void delays()
	{
	  uchar t,ms;
	  ms = 10;
	  while(ms--)for(t=0;t<120;t++);
	}
	
void main()
	{
	  uchar DIR=0;
	  uchar speed=3;
	  char m,n;
	  uchar i,num;
	  uchar temp;
	  P1=0x7f;
	  
	  while(1)
	  {
		  //这里0xf0就是1的意思，0x30就是0的意思
	    P3=0xf0;
		temp=P3;
		temp=temp&0x30;
		if(temp!=0x30)
		{
			//这是按键扫描程序
		  delays();
		  P3=0xf0;
		  temp=P3;
		  temp=temp&0x30;
		  if(temp!=0x30)
		  {
		    key=temp;
			temp=temp|0x03;
			P3=temp;
			temp=P3&0x03;
			key=temp|key;
			key=0x33-key;
			P3=0xf0;
			for(i=0;i<4;i++)
			{
				if(key==KEY_TABLE[i])
				
			{
					num=i;
					break;
			}
			}
		switch(num)
		{
			
			case 0:
			{
			speed++;
			//对于溢出的处理
			if(speed==6){speed=1;}break;
			}
			case 1:
			{
			speed--;
			if(speed==0){speed=5;}break;
			}
			case 2:DIR;break;
			case 3:DIR;break;
		}
		
		switch(speed)
		{
			//将这数段按照数组的形式放入P0各个口中
			case 1:P0=0xFE;break;
			case 2:P0=0xFD;break;
			case 3:P0=0xFB;break;
			case 4:P0=0xF7;break;
			case 5:P0=0xEF;break;
		}
		if(DIR==1)
			{
			   for(;m<8;m++)
			   {P1=OUT_TABLE[m];
			   //用这个来控制速度
			   delay(speed);
			   
			   if(m==7){m=-1;}
			   temp=P3&0x30;
			   if(temp!=0x30){break;}
			   }
		   }
	   if(DIR==0)
		{
			for(;m<8;m--)
			{
				P1=OUT_TABLE[m];
				delay(speed);
				if(m==0){m=8;}
				temp=P3&0x30;
				if(temp!=0x30){break;}
			}
		}
	}
}
}
}