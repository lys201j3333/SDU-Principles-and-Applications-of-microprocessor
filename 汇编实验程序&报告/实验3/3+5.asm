DATAS SEGMENT
     data1 DB 255 dup('$')   
     data2 DB 255 dup('$') 
	 str1  DB 'please input first number: $ '  
	 str2  DB 'please input second number: $ ' 
	 str3  DB 'the result:$'
	 enter  DB  0DH,0Ah ,'$'
DATAS ENDS


STACK SEGMENT
	lys DB 255 dup(0)  
STACK ENDS


CODES SEGMENT
     ASSUME DS:DATAS,CS:CODES
START:
    MOV AX, DATAS
    MOV DS, AX
	
	LEA DX,str1
	MOV AH,09H
	INT 21H


	LEA DX,data1
	MOV AH,0AH
	INT 21H
	
	LEA DX,enter
	MOV AH,09H
	INT 21H
	
	MOV BL, [data1+2]
	SUB BL,'0'
	MOV BH, [data1+3]
	SUB BH,'0'
	
	
	LEA DX,str2
	MOV AH,09H
	INT 21H
	
	
	LEA DX,data2
	MOV AH,0AH
	INT 21H
	
	LEA DX,enter
	MOV AH,09H
	INT 21H
	
	MOV CL,[data2+2]
	SUB CL,'0'
	MOV CH,[data2+3]
	SUB CH,'0'
	
	
	ADD BL,CL
	ADD BH,CH

	LEA DX,str3
	MOV AH,09H
	INT 21H

	CMP BH,10
	JGE single_adding
tens_adding:
	CMP BL,10
	JGE input_num
	JMP input_num2
	
single_adding:
	SUB BH,10
	ADD BL,1
	JMP tens_adding
input_num:

	SUB BL,10
	MOV DL,1
	ADD DL,30H
	MOV AH,02H
	INT 21H
	JGE input_num2
input_num2:
	MOV DL,BL
	ADD DL,30H
	MOV AH,02H
	INT 21H
	MOV DL,BH
	ADD DL,30H
	MOV AH,02H
	INT 21H
	
	MOV AH,4CH
	INT 21H
CODES ENDS
      END START