DATAS SEGMENT
    string_a db 'The School of Information Science and Engineering Shandong University',13,10,'$' 
DATAS ENDS

EXT SEGMENT
    string_b db 100 dup(0) 
    string_c db 100 dup(0)
EXT ENDS

STACK SEGMENT STACK
    db 2000 dup(0)
STACK ENDS

CODE SEGMENT
      assume ds:datas,ss:stack,cs:code,es:ext
  start:
    mov ax,datas
    mov ds,ax
    mov ax,ext
    mov es,ax
     
    lea dx,string_a
    mov ah,9
    int 21h
    
    ;call duplicate
    ;call turn_duplicate

duplicate proc near
    lea si,string_a - 1
    mov sp,0000h
    mov dx,0
tt1:inc si
    inc dx 
    mov ax,[si] 
    push ax
    cmp ax,24h
    je output 
    loop tt1 
    
output:
    lea si,string_b - 1
    mov bx,0
tt2:inc si
    inc bx
    pop [si]
    mov ax,[si]
    cmp bx,dx
    je  display
    loop tt2   
    
display:
    lea dx,string_b 
    mov ah,9
    int 21h
    jmp exit  
    
exit:
    mov ah,4ch
    int 21h        
        
    
           
    
turn_duplicate proc near  
code ends 
 end start
          