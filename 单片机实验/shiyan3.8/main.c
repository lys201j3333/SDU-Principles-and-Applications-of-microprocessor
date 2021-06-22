#include <reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
uchar data line_data[16];
uchar code numchar[] = "0123456789";
sbit LCD_RS = P2 ^ 0;
sbit LCD_RW = P2 ^ 1;
sbit LCD_EN = P2 ^ 2;
sbit AD_CLK = P2 ^ 3;
sbit Start = P2 ^ 4;
sbit OE = P2 ^ 5;
sbit Out_pulse = P2 ^ 6;
sbit EOC = P2 ^ 7;
uint tmpint, number, c;
uchar samp_data, tmpchar, n, t, a, b, d, p1_tmp;

void delay_ms(uint xms)
{ //延时子程序
	uint j;
	while (xms--)
		for (j = 110; j > 0; j--)
			;
}

bit lcd_busy()
{ //LCD忙检查子程序，最高位BF=1表示液晶显示器忙，暂时无法接收单片机送来的数据或指令
	bit flag;
	p1_tmp = P1;//保护P1口的值，防止其被修改（推测是Proteus8的问题）
	LCD_RS = 0;
	LCD_RW = 1;
	LCD_EN = 1;
	_nop_(); //如果单片机的晶振是12M，则调用_nop_()用于延时1us
	_nop_();
	_nop_();
	_nop_();
	flag = (bit)(P1 & 0x80); //返回最高位BF代表是否忙，忙flag=1，不忙flag=0
	LCD_EN = 0;
	P1 = p1_tmp;
	return flag;
}

//写命令子程序
void lcd_wcmd(uchar cmd)
{
	while (lcd_busy());
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;
	_nop_();
	_nop_();
	P1 = cmd; //P1口传输命令到LCD1602
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN = 0; //下降沿脉冲
}

void lcd_clr()
{
	lcd_wcmd(0x01); //清屏命令
	delay_ms(2);	//清屏命令需要1.64ms以上
}

//写数据子程序
void lcd_wdat(uchar dat)
{
	while (lcd_busy());
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EN = 0;
	_nop_();
	_nop_();
	P1 = dat; //P1口传输数据到LCD1602
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD_EN = 0; //下降沿脉冲
}

void lcd_init()
{
	delay_ms(5);	//等待LCD电源稳定
	lcd_wcmd(0x01); //清除LCD显示内容
	delay_ms(5);
	lcd_wcmd(0x06); //移动光标
	delay_ms(5);
	lcd_wcmd(0x0c); //显示开，关光标，不闪烁
	delay_ms(5);
	lcd_wcmd(0x38); //8位数据总线，2行，5*7点阵每字符
	delay_ms(15);
}

void showstring()
{
	uchar i = 0;
	lcd_wcmd(0x80); //设置显示位置为第1行第0列
	while (line_data[i] != '\0')
	{
		lcd_wdat(line_data[i]);
		delay_ms(5);
		i++;
	}
}

void s_timer0() interrupt 1
{ //定时器T0实现中断，完成50ms定时
	t++;
	TH0 = 0x3c; //T0采用方式1，需要重装初值
	TL0 = 0xb0;
}

void s_timer1() interrupt 3
{ //定时器T1实现中断，为50kHz
	AD_CLK = ~AD_CLK; //取反
}

void sample()
{
	TR0 = 0; //关闭定时器T0中断，避免该子程序对采集精度的影响，因为此子程序是用用于处理采集结果的
	if (samp_data > 128 && b == 0)
	{	//此处引入变量a,b用来找出零点，假如第一次采集结果samp_data大于128，则采集结果为正数，则b赋1，当下一次采集结果用于此处判断时一定不会执行判断后
		//的指令，因为只有当b = 0时才会执行这个判断；而下一个判断却可以将b赋为0，执行下一个判断，采集结果必须为负，由此实现对零点的计数
		number++;
		a = 0;
		b = 1;
		Out_pulse = 1; //信号过零点，产生5us脉冲信号
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		Out_pulse = 0;
	}
	else if (samp_data <= 128 && a == 0)
	{
		number++;
		a = 1;
		b = 0;
		Out_pulse = 1; //信号过零点，产生5us脉冲信号
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		Out_pulse = 0;
	}
	TR0 = 1; //打开定时器T0中断
}

int main(void)
{
	delay_ms(10);
	lcd_init(); //初始化
	lcd_clr();	//清屏
	delay_ms(2);
	t = 0;
	a = 0;
	b = 0;
	n = 0;
	line_data[n++] = 'f'; //先将要在LCD1602上输出的字符串的前三个值赋为'f：'
	line_data[n++] = ':';
	line_data[n++] = ' ';
	Out_pulse = 0;
	number = 0;
	AD_CLK = 0;
	OE = 0;		 //关闭ADC0808的输出使能
	TMOD = 0x21; //T0工作在方式1，T1工作在方式2,T1不需要重装初值
	TH1 = 0xF6;	 //T1 50kHz
	TL1 = 0xF6;
	TH0 = 0x3C; //T050ms定时
	TL0 = 0xB0;
	EA = 1; //打开中断总开关，下面就是打开定时器T0中断，定时器T1中断
	ET0 = 1;
	TR0 = 1;
	ET1 = 1;
	TR1 = 1;
	while (1)
	{
		Start = 1; //启动AD
		Start = 0; //启动信号结束
		while (EOC != 1)
			;	//EOC转换结束为1
		OE = 1; //打开ADC0808的输出使能
		P0 = 0xff;
		samp_data = P0; //采集
		OE = 0;			//关闭ADC0808的输出使能
		sample();		//处理采集结果
		if (t >= 9)
		{			 //频率等于1s内的零点数除2，但是考虑到除2可能会产生余数，所以此处记录的0.5s的零点数，频率就直接等于0.5s的零点数
			TR0 = 0; //关闭定时器T0中断，避免该步骤对采集精度的影响，因为此步骤是用于计算频率
			t = 0;
			tmpint = number;
			number = 0;
			n = 3; //前面输出字符串中已经先赋值了前三个字符，此处要从第四个字符开始赋值
			d = 0;
			a = 0;
			b = 0;
			c = 100;
			while (c != 0)
			{
				tmpchar = (uchar)(tmpint / c);
				tmpint = tmpint - (uint)tmpchar * c;
				c /= 10;
				if (tmpchar != 0) //输出计算得到的频率，此处tmpchar为显示的数据，为了避免输出多余的零在最终输出数据前面，所以引入了d用于标记有效数字
					d = 1;
				if (d != 0)
					line_data[n++] = numchar[tmpchar];
			}
			line_data[n++] = 'H';
			line_data[n++] = 'z';
			line_data[n] = '\0'; //将字符串补充完整
			
			showstring();
			TR0 = 1; //打开定时器T0中断
		}
	}
}