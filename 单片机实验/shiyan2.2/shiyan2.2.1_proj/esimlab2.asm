    ORG 0000H
        AJMP MAIN
    ORG 0030H
MAIN:
        MOV 30H, #45H
        MOV A, 30H
        ANL A,#0FH
        MOV 31H,A
        MOV A,30H

        ANL A, #0F0H
        SWAP A
        MOV B, #10
        MUL AB
        ADD A,31H
        MOV 31H,A
        SJMP $
        END