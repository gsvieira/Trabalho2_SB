section .data
acc dd 0
label1 dd 0
label0 dd 0
label2 dd 2

section .text

global _start
_start:
call input_function

call input_function

;LOAD
mov eax, [label1]
mov [acc], eax

mov eax, [acc]
imul dword [label0]
mov [acc], eax

mov eax, [acc]
cdq
idiv dword [label2]
mov [acc], eax

mov eax, [acc]
mov [label1], eax

call output_function

mov eax, 1
mov ebx, 0
int 80h

input_function:
enter 0,0
mov eax, 3
mov ebx, 0
mov ecx,
mov edx,
int 80h
leave
ret

s_input_function:
enter 0,0
mov eax, 3
mov ebx, 0
mov ecx, [ebp+12]
mov edx, [ebp+8]
int 80h
leave
ret

s_output_function:
enter 0,0
mov eax, 4
mov ebx, 1
mov ecx, [ebp+12]
mov edx, [ebp+8]
int 80h
leave
ret

