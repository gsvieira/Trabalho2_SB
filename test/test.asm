section .text

global _start

_start:

    enter 0,0
    mov eax, 3
    mov ebx, 0
    mov ecx, aux
    mov edx, 12
    int 80h
    push eax

    mov eax, 0
    mov esi, 0
    mov ecx, 0

input_loop:
    mov ebx, 0
    mov bl, [aux+esi]
    inc esi
    cmp bl, '-' ; if neg
    je input_neg
    cmp bl, '0'
    jb input_end
    cmp bl, '9'
    jg input_end
    sub bl, '0'
    mul dword [ten]
    add eax, ebx
    jmp input_loop
input_neg:
    mov ecx,1
    jmp input_loop


input_end:
    cmp ecx, 1
    jne input_cleanup
    neg eax

input_cleanup:
    mov [ebp+8], eax
    pop eax
    leave
    ret


section .data

    ten dd 10

segment .bss

    aux resb 12
