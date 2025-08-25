BITS 16
SECTION MBR vstart=0x7c00

;稳妥使用栈
mov ax, cx
mov ds, ax
mov es, ax
mov ss, ax
mov cx, ax
mov sp, 0x7c00

counter dw 0x00;定义数据段

start:
    cli ;关中断

    ;定时器提供的软中断地址是70-73，定时器地址是20-23
    mov word [0x70], clock_handler ;修改中断，跳转clock_handler处理中断
    mov word [0x72], 0

    ; 设置定时器0（8253/8254）以产生时钟中断

    ; 发送命令字节到控制寄存器端口0x43
    mov al, 00110110b ; 方式3，用于定时产生中断
    out 0x43, al

    ; 计算计数值，产生20毫秒的时钟中断，时钟频率为1193180赫兹
    ; 计数值 = (时钟频率 / 每秒中断次数) - 1 = (1193180 / (1 / 0.02)) - 1 = 23863
    mov ax, 23863

    ; 将计数值分为低字节和高字节，发送到计数器0的数据端口（端口0x40）
    out 0x40, al        ; 低字节
    mov al, ah
    out 0x40, al        ; 高字节
    xor cx,cx
    push cx
    mov bp, sp
    sti ;开中断
    jmp $   ;使程序不断停在这里执行等待时钟中断

; 时钟中断处理例程
clock_handler:
    inc word [bp]
    mov cx, [bp]
    cmp cx, 50
    jne no_print
    mov si,msg
    call print
    mov word [counter], 0
    xor cx, cx
    mov word [bp], 0
    iret

no_print:
    iret

exit:
    ret

print:
    ; 打印字符
    lodsb
    or al, al
    jz exit
    mov ah, 0x0E
    int 0x10          ; 调用INT 10h中断
    jmp print
    
msg db "Yong-Ming Tian ", 0

times 510-($-$$) db 0  ; 填充剩余的512字节以使文件大小为512字节
dw 0xAA55              ; MBR结束标志