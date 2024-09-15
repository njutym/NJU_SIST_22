BITS 16
section .text
global write
global sleep

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

;不够好的实现，通过反汇编找回去的指令跳转，与现代操作系统保护模式下内核回app不能用jmp不符合，舍弃不用
not_that_good_sleep:
    push ebp
    mov ebp,esp
    push eax
    push ecx

    mov eax,[ebp+8] ;获取参数

    push 0x80ab ;通过反汇编得到，对应的int 0x81后的那条语句

    mov ecx, esp
    mov esp, 0x1000
    push ecx
    int 0x81
    pop ecx
    pop eax
    pop ebp
    retd

;好的实现,通过进程切换回跳main，这在内核代码体现了
sleep:
    push ebp
    mov ebp,esp
    push eax

    mov eax,[ebp+8] ;获取参数

    int 0x81
    
    pop eax
    pop ebp
    retd

