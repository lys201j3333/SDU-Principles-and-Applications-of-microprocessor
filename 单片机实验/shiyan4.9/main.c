#include <reg51.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
#define LCD_Data P1
sbit LCD_RS = P0 ^ 3;
sbit LCD_WR = P0 ^ 2;
sbit LCD_EN = P0 ^ 1;
sbit PSB = P0 ^ 0;
uchar code name[] = "李禹申";
uchar code number[] = "201900120123";
uchar code a[] = "123";

/**********************************延时子程序**********************/
void delay_ms(uint xms)
{
    uint j;
    while (xms--)
        for (j = 110; j > 0; j--);
}

void Write_Data(uchar dat)
{
    LCD_EN = 0;
    LCD_WR = 0; //选择写模式
    LCD_RS = 1; //选择数据模式
    //延时
    delay_ms(1);
    //将数据准备
    LCD_Data = dat;
    LCD_EN = 1;
    _nop_();
    _nop_();
    LCD_EN = 0;  //表示开始写入数据,下降沿写入数据
    delay_ms(2); //延时等待数据全部发送完成
}

//向12864中写入命令
void Write_Com(uchar com)
{
    //首先我们设置片选信号
    LCD_WR = 0; //选择写模式
    LCD_RS = 0; //选择命令模式
    //延时
    delay_ms(1);
    //将数据准备
    LCD_Data = com;
    LCD_EN = 1; //片选信号一般设置为0表示片选
    _nop_();
    _nop_();
    LCD_EN = 0;  //表示开始写入数据,下降沿写入命令
    delay_ms(2); //延时等待数命令全部发送完成
}

//对LCD12864进行初始化
void InitLCD()
{
    PSB = 1; //并口方式
    LCD_WR = 0;
    LCD_EN = 0;
    Write_Com(0x30); //设置为8位并行口，基本指令集动作
    Write_Com(0x08); //整体显示关，游标关，游标位置关
    Write_Com(0x01); //清除LCD的显示内容
    Write_Com(0x06); //显示光标右移，DDRAM位地址加一，画面不移动
    Write_Com(0x0c); //开显示，关光标
}

//汉字一行能显示8个，字符能显示16个
/*
第一行：0x80 -> 0x87
第二行：0x90 -> 0x97
第三行：0x88 -> 0x8f
第四行：0x98 -> 0x9f
*/
unsigned char pos[] = {0x80, 0x90, 0x88, 0x98};

void Display(uchar x, uchar y, uchar code *str)
{
    uchar i;
    //写入地址
    Write_Com(pos[x] + y);
    //写入数据
    for (i = 0; str[i] != '\0'; i++)
        Write_Data(str[i]);
}

int main(void)
{
    //液晶初始化
    InitLCD();
    while (1)
    {
        Display(0, 0, name); //名字写在第一行
        Display(1, 0, number); //学号写在第二行
		Display(2, 0, a);
    }
}