；是一个伪指令，用来定义一个堆栈段
STACKS SEGMENT STACK
；① DW是定义字类型变量的意思（问题①） DUP是连续复制128个变量，(?)代表所赋值数值为随机
  DW 128 DUP(?)
；结束堆栈段的定义
STACKS ENDS

；开始DATA段的定义，SEGMENT是定义段
DATAS SEGMENT
；string是一个标识符，标记给这个空间的首地址，13是ASCII中的回车 10是ASCII中的换行 '$'是汇编中字符串结束的标志
  STRING DB 13,10,'Hello World!',13,10,'$'
；DATA段结束
DATAS  ENDS

；CODE段开始定义
CODES SEGMENT
；ASSUME建立段寄存器寻址，将CODE段和CS段寄存器相关联，DATAS段和DS段寄存器相关联
  ASSUME  CS:CODES,DS:DATAS
；入口地址
START:
；下面两行将DATAS地址放到DS段寄存器中，之所以要AX过渡，是因为不能直接对段寄存器给数
  MOV  AX,DATAS
  MOV  DS,AX
；将STRING所在的内存地址赋给DX
  LEA  DX,STRING
；下面两条同时作用可以让其显示字符串
  MOV  AH,9
  INT 21H
；下面两条指令是返回DOS
  MOV  AH,4CH
  INT 21H
；CODES段结束，出口
CODES ENDS
  END START