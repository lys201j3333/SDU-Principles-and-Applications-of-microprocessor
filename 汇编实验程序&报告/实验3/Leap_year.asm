data segment
    infon db 0dh,0ah,'Please input a year:$'
    Y db 0dh,0ah,'Wow! This is a leap year!$'
    N db 0dh,0ah,'sorry, this is not a leap year!$'
    w dw 0
    buf db 8
        db 0
        db 8 dup(0)
data ends

stack segment stack
    db 200 dup(0)
stack ends

code segment
          assume ds:data,ss:stack,cs:code
    start:
        mov ax,data
        mov ds,ax
        
        lea dx,infon 
        mov ah,9
        int 21h
        
        lea dx,buf
        mov ah,10
        int 21h
        
        mov cl, [buf+1]
        mov ch,0
        
        lea di,buf+2
        call datacate
        call ifyears
        jc   a1
        
        lea dx,n
        mov ah,9
        int 21h
        jmp exit
    a1:
        lea dx,y
        mov ah,9
        int 21h
    exit:
        mov ah,4ch
        int 21h   


datacate proc near
         push cx
         dec cx
         lea si,buf+2
    tt1: inc si
         loop tt1
         pop cx
         
         
         mov dh,30h
         mov bl,10
         mov ax,1
    l1:push ax
    push bx
    push dx
    
         sub byte ptr [si],dh
         mov bl,byte ptr[si]
    mov  bh,0
        mul  bx
        add [w],ax
    pop dx
    pop bx
        pop ax
        mul bl
        dec si
        loop l1
        ret
datacate endp


ifyears proc near
        push bx
        push cx
        push dx
        mov ax,[w]
        mov cx,ax
        mov dx,0
        
        mov bx,100  
        div bx  
        cmp dx,0
        jnz lab1
        mov ax,cx
        mov bx,400
        div bx
        cmp dx,0
        jz lab2
        clc
        jmp lab3 
        
        lab1:
            mov ax,cx
            mov dx,0
            mov bx,4
            div bx
            cmp dx,0
            jz lab2
            clc
            jmp lab3
        lab2:stc
        lab3:
            pop dx
            pop cx
            pop bx
            ret    
        ifyears endp
code ends
    end start