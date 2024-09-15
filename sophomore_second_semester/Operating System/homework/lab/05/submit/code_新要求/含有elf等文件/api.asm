BITS 16
section .text
global write
global sleep
global fork

write:
    ;保护栈
    push ebp
    mov  ebp, esp
    push ebx
    push ecx

    mov  ebx, [ebp+8] ;地址
    mov  ecx, [ebp+12] ;长度
    int 0x80    ;调用中断

    pop ecx
    pop ebx
    pop ebp
    retd    ;回跳要用32位的指令，ret是16位的

;好的实现,通过进程切换回跳main，这在内核代码体现了
sleep:
    push ebp
    mov ebp,esp
    push ebx

    mov ebx,[ebp+8] ;获取参数

    int 0x81
    
    pop ebx
    pop ebp
    retd

fork:
    ;ax是返回值,先返回父进程1
    mov eax,1
    push eax

    int 0x82

    pop eax
    retd


