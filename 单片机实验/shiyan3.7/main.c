#include <reg51.h>		  //头文件
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

sbit lcden = P1 ^ 5; //定义引脚
sbit rs = P1 ^ 7;
sbit rw = P1 ^ 6;
sbit busy = P0 ^ 7;

char i, j, temp, num;
long a, b, c, d; 
float a_c, b_c;
uchar flag, op, p2_tmp; //flag表示是否有符号键按下，op表征按下的是哪个符号

uchar code table[] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 0, 0,
	0, 0, 0, 0};
uchar code table1[] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	8, 9, 0x2b - 0x30, 0x2d - 0x30,
	0x2a - 0x30, 0x2f - 0x30, 0x01 - 0x30, 0x3d - 0x30};
uchar code table2[] = "!rorrE"; //第二行从右向左输出，因此用倒序

void delay(uchar z) // 延迟函数
{
	uchar y;
	for (z; z > 0; z--)
		for (y = 0; y < 110; y++)
			;
}

void check() // 判断忙或空闲
{
	do
	{
		P2 = 0xFF;
		rs = 0;			 //指令
		rw = 1;			 //读
		lcden = 0;		 //禁止读写
		delay(1);		 //等待，液晶显示器处理数据
		lcden = 1;		 //允许读写
	} while (busy); //判断是否为空闲，1为忙，0为空闲
}

void write_com(uchar com) // 写指令函数
{
	P2 = com; //com指令付给P0口
	rs = 0;
	rw = 0;
	lcden = 0;
	check();
	lcden = 1;
}

void write_data(uchar data_) // 写数据函数
{
	P2 = data_;
	rs = 1;
	rw = 0;
	lcden = 0;
	check();
	lcden = 1;
}

void init() //初始化
{
	num = -1;
	lcden = 1;		 //使能信号为高电平
	write_com(0x38); //8位，2行
	write_com(0x0c); //显示开，光标关，不闪烁
	write_com(0x06); //增量方式不移位
	write_com(0x80); //检测忙信号
	write_com(0x01); //显示开，光标关，不闪烁
	i = 0;
	j = 0;
	a = 0; //第一个参与运算的数
	b = 0; //第二个参与运算的数
	c = 0; //运算结果
	d = 0; //整除的余数
	flag = 0;  //flag表示是否有符号键按下，
	op = 0; // op表征按下的是哪个符号
}

void keyscan() // 键盘扫描程序
{
	P3 = 0xfe;
	if (P3 != 0xfe)
	{
		delay(20);
		if (P3 != 0xfe)
		{
			temp = P3 & 0xf0;
			switch (temp)
			{
			case 0xe0:
				num = 0;
				break;
			case 0xd0:
				num = 1;
				break;
			case 0xb0:
				num = 2;
				break;
			case 0x70:
				num = 3;
				break;
			}
		}
		while (P3 != 0xfe)
			;
		if (flag == 0) //没有按过符号键
		{
			a = a * 10 + table[num];
		}
		else //如果按过符号键
		{
			b = b * 10 + table[num];
		}
		i = table1[num];
		write_data(0x30 + i);
	}

	P3 = 0xfd;
	if (P3 != 0xfd)
	{
		delay(5);
		if (P3 != 0xfd)
		{
			temp = P3 & 0xf0;
			switch (temp)
			{
			case 0xe0:
				num = 4;
				break;

			case 0xd0:
				num = 5;
				break;

			case 0xb0:
				num = 6;
				break;

			case 0x70:
				num = 7;
				break;
			}
		}
		while (P3 != 0xfd)
			;
		if (flag == 0) //没有按过符号键
		{
			a = a * 10 + table[num];
		}
		else //如果按过符号键
		{
			b = b * 10 + table[num];
		}
		i = table1[num];
		write_data(0x30 + i);
	}

	P3 = 0xfb;
	if (P3 != 0xfb)
	{
		delay(5);
		if (P3 != 0xfb)
		{
			temp = P3 & 0xf0;
			switch (temp)
			{
			case 0xe0:
				num = 8;
				break;

			case 0xd0:
				num = 9;
				break;

			case 0xb0:
				num = 10;
				break;

			case 0x70:
				num = 11;
				break;
			}
		}
		while (P3 != 0xfb)
			;
		if (num == 8 || num == 9) //如果按下的是'8','9'
		{
			if (flag == 0) //没有按过符号键
			{
				a = a * 10 + table[num];
			}
			else //如果按过符号键
			{
				b = b * 10 + table[num];
			}
		}
		else if (num == 10) //如果按下的是'+'
		{
			flag = 1;
			op = 1; //2表示加号已按
		}
		else if (num == 11) //如果按下的是'-'
		{
			flag = 1;
			op = 2; //2表示减号已按
		}
		i = table1[num];
		write_data(0x30 + i);
	}

	P3 = 0xf7;
	if (P3 != 0xf7)
	{
		delay(5);
		if (P3 != 0xf7)
		{
			temp = P3 & 0xf0;
			switch (temp)
			{
			case 0xe0:
				num = 12;
				break;

			case 0xd0:
				num = 13;
				break;

			case 0xb0:
				num = 14;
				break;

			case 0x70:
				num = 15;
				break;
			}
		}
		while (P3 != 0xf7)
			;
		switch (num)
		{
			case 12:
			{
				write_data(0x30 + table1[num]);
				flag = 1;
				op = 3;
			}
			break;
			case 13:
			{
				write_data(0x30 + table1[num]);
				flag = 1;
				op = 4;
			}
			break;
			case 14:
			{
				write_com(0x01);
				i = 0;
				j = 0;
				a = 0;
				b = 0;
				c = 0;
				d = 0;
				flag = 0;
				op = 0;
			} //按下的是"清零"
			break;
			case 15:
			{
				j = 1;
				if (op == 1)
				{
					write_com(0x80 + 0x4f); //按下等于键，光标前进至第二行最后一个显示处
					write_com(0x04);		//设置从后往前写数据，每写完一个数据，光标后退一格
					c = a + b;
					while (c != 0)
					{
						write_data(0x30 + c % 10);
						c = c / 10;
					}
					write_data(0x3d); //再写"="
					a = 0;
					b = 0;
					flag = 0;
					op = 0;
				}
				else if (op == 2)
				{
					write_com(0x80 + 0x4f); //光标前进至第二行最后一个显示处
					write_com(0x04);		//设置从后往前写数据，每写完一个数据，光标后退一格)
					if (a - b > 0)
						c = a - b;
					else
						c = b - a;
					while (c != 0)
					{
						write_data(0x30 + c % 10);
						c = c / 10;
					}
					if (a - b < 0)
						write_data(0x2d);
					write_data(0x3d); //再写"="
					a = 0;
					b = 0;
					flag = 0;
					op = 0;
				}
				else if (op == 3)
				{
					write_com(0x80 + 0x4f);
					write_com(0x04);
					c = a * b;
					while (c != 0)
					{
						write_data(0x30 + c % 10);
						c = c / 10;
					}
					write_data(0x3d);
					a = 0;
					b = 0;
					flag = 0;
					op = 0;
				}
				else if (op == 4)
				{
					write_com(0x80 + 0x4f);
					write_com(0x04);
					if (b == 0) //除数为零
					{
						i = 0;
						while (table2[i] != '\0')
						{
							write_data(table2[i]);
							i++;
						} //输出错误信息
						a = 0;
						b = 0;
						flag = 0;
						op = 0;
					}
					else if ((a % b == 0) && (b != 0)) //可以整除
					{
						c = a / b;
						while (c != 0)
						{
							write_data(0x30 + c % 10);
							c = c / 10;
						} //输出商
						if (a / b <= 0)
							write_data(0x30);
						write_data(0x3d);
						a = 0;
						b = 0;
						flag = 0;
						op = 0;
					}
					else if ((a % b != 0) && (b != 0)) //不能整除
					{
						d = a % b;
						while (d != 0)
						{
							write_data(0x30 + d % 10);
							d = d / 10;
						} //输出余数
						write_data(0x2e);
						write_data(0x2e);
						write_data(0x2e); //三个点分隔
						c = a / b;
						while (c != 0)
						{
							write_data(0x30 + c % 10);
							c = c / 10;
						}
						if (a / b <= 0)
							write_data(0x30);
						write_data(0x3d);
						a = 0;
						b = 0;
						flag = 0;
						op = 0;
					}
				}
			}
			break;
		}
	}
}

int main(void)
{
	init();
	while (1)
	{
		keyscan();
	}
	return 0;
}
