#include <reg51.h>		  //ͷ�ļ�
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

sbit lcden = P1 ^ 5; //��������
sbit rs = P1 ^ 7;
sbit rw = P1 ^ 6;
sbit busy = P0 ^ 7;

char i, j, temp, num;
long a, b, c, d; 
float a_c, b_c;
uchar flag, op, p2_tmp; //flag��ʾ�Ƿ��з��ż����£�op�������µ����ĸ�����

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
uchar code table2[] = "!rorrE"; //�ڶ��д����������������õ���

void delay(uchar z) // �ӳٺ���
{
	uchar y;
	for (z; z > 0; z--)
		for (y = 0; y < 110; y++)
			;
}

void check() // �ж�æ�����
{
	do
	{
		P2 = 0xFF;
		rs = 0;			 //ָ��
		rw = 1;			 //��
		lcden = 0;		 //��ֹ��д
		delay(1);		 //�ȴ���Һ����ʾ����������
		lcden = 1;		 //�����д
	} while (busy); //�ж��Ƿ�Ϊ���У�1Ϊæ��0Ϊ����
}

void write_com(uchar com) // дָ���
{
	P2 = com; //comָ���P0��
	rs = 0;
	rw = 0;
	lcden = 0;
	check();
	lcden = 1;
}

void write_data(uchar data_) // д���ݺ���
{
	P2 = data_;
	rs = 1;
	rw = 0;
	lcden = 0;
	check();
	lcden = 1;
}

void init() //��ʼ��
{
	num = -1;
	lcden = 1;		 //ʹ���ź�Ϊ�ߵ�ƽ
	write_com(0x38); //8λ��2��
	write_com(0x0c); //��ʾ�������أ�����˸
	write_com(0x06); //������ʽ����λ
	write_com(0x80); //���æ�ź�
	write_com(0x01); //��ʾ�������أ�����˸
	i = 0;
	j = 0;
	a = 0; //��һ�������������
	b = 0; //�ڶ��������������
	c = 0; //������
	d = 0; //����������
	flag = 0;  //flag��ʾ�Ƿ��з��ż����£�
	op = 0; // op�������µ����ĸ�����
}

void keyscan() // ����ɨ�����
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
		if (flag == 0) //û�а������ż�
		{
			a = a * 10 + table[num];
		}
		else //����������ż�
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
		if (flag == 0) //û�а������ż�
		{
			a = a * 10 + table[num];
		}
		else //����������ż�
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
		if (num == 8 || num == 9) //������µ���'8','9'
		{
			if (flag == 0) //û�а������ż�
			{
				a = a * 10 + table[num];
			}
			else //����������ż�
			{
				b = b * 10 + table[num];
			}
		}
		else if (num == 10) //������µ���'+'
		{
			flag = 1;
			op = 1; //2��ʾ�Ӻ��Ѱ�
		}
		else if (num == 11) //������µ���'-'
		{
			flag = 1;
			op = 2; //2��ʾ�����Ѱ�
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
			} //���µ���"����"
			break;
			case 15:
			{
				j = 1;
				if (op == 1)
				{
					write_com(0x80 + 0x4f); //���µ��ڼ������ǰ�����ڶ������һ����ʾ��
					write_com(0x04);		//���ôӺ���ǰд���ݣ�ÿд��һ�����ݣ�������һ��
					c = a + b;
					while (c != 0)
					{
						write_data(0x30 + c % 10);
						c = c / 10;
					}
					write_data(0x3d); //��д"="
					a = 0;
					b = 0;
					flag = 0;
					op = 0;
				}
				else if (op == 2)
				{
					write_com(0x80 + 0x4f); //���ǰ�����ڶ������һ����ʾ��
					write_com(0x04);		//���ôӺ���ǰд���ݣ�ÿд��һ�����ݣ�������һ��)
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
					write_data(0x3d); //��д"="
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
					if (b == 0) //����Ϊ��
					{
						i = 0;
						while (table2[i] != '\0')
						{
							write_data(table2[i]);
							i++;
						} //���������Ϣ
						a = 0;
						b = 0;
						flag = 0;
						op = 0;
					}
					else if ((a % b == 0) && (b != 0)) //��������
					{
						c = a / b;
						while (c != 0)
						{
							write_data(0x30 + c % 10);
							c = c / 10;
						} //�����
						if (a / b <= 0)
							write_data(0x30);
						write_data(0x3d);
						a = 0;
						b = 0;
						flag = 0;
						op = 0;
					}
					else if ((a % b != 0) && (b != 0)) //��������
					{
						d = a % b;
						while (d != 0)
						{
							write_data(0x30 + d % 10);
							d = d / 10;
						} //�������
						write_data(0x2e);
						write_data(0x2e);
						write_data(0x2e); //������ָ�
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
