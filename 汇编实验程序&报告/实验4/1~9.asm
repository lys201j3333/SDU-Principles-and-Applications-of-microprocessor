DATAS SEGMENT
   BUF1 DB 13,10,'THE Number is ','$' 
DATAS ENDS
STACKS SEGMENT STACK
   DB 100 DUP(0)
STACKS ENDS
CODES SEGMENT
   ASSUME DS:DATAS,SS:STACKS,CS:CODES

START:
   MOV AX,DATAS
   MOV DS,AX

   LEA DX,BUF1
   MOV AH,9
   INT 21H

   MOV AX,0
   MOV DS,AX 

   MOV BX,86H*4

   CLI 

   MOV WORD PTR DS:[BX],OFFSET NUMBER

   MOV WORD PTR DS:[BX+2],SEG NUMBER
  
   STI 

   INT 86H  

   MOV AH,4CH
   INT 21H  

NUMBER PROC

   PUSH CX
   PUSH DX 

   MOV AH,0
   INT 1AH   


   MOV AX,DX 
   MOV DX,0
   MOV BX,10
   DIV BX

   ADD DL,30H

   MOV AH,02H
   INT 21H 


   POP DX
   POP CX 
   IRET     
NUMBER ENDP  

CODES ENDS
END START