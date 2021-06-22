DATAS SEGMENT
    
CODE SEGMENT
    ASSUME CS: CODE
START:
    MOV AH,07H
    INT 21H
    MOV DL,AL 
    MOV BL,DL
S: CMP DL,39H
   JGE DECIDE41
   JMP DECIDE31 
loop s 
DECIDE31: CMP DL,31H
          JGE ONENINE
          JMP DECIDEOD
DECIDEOD: CMP DL,0DH
          JGE JEND
          JMP NONE
DECIDE41: CMP DL,41H
          JGE DECIDE5A
          JMP START
DECIDE5A: CMP DL,5AH
          JGE DECIDE61
          JMP OUTPUTC
DECIDE61: CMP DL,61H
          JGE DECIDE7A
          JMP NONE
DECIDE7A: CMP DL,7AH
          JGE NONE
          JMP OUTPUTC                             
                            

ONENINE:MOV AH,02H
        INT 21H
        JMP START

OUTPUTC:MOV DL,63H
        MOV AH,02H
        INT 21H
NONE:   JMP START                
JEND:   MOV AH, 4CH
        INT 21H        
CODE ENDS
    END START