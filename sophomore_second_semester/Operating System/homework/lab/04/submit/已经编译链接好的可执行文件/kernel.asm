section .text

BITS 16
SECTION MBR vstart=0x7c00

    ;调试counter变化是否正确
    ;mov ah, 0x0E    ; AH = 0x0E 表示打印字符功能
    ;mov bx, 0x0000  ; BH = 0x00 表示页面编号（单色文本模式下为0），BL = 0x00 表示颜色（默认颜色）
    ;mov cx, [counter]
    ;add cx, '0'     ; 将计数器的值转换为对应的 ASCII 字符
    ;mov al, cl      ; 将转换后的字符存放在 AL 中
    ;int 0x10        ; 调用BIOS中断以显示字符

_start:
    ;初始化栈
    xor ax, ax 
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov cx, ax
    mov sp, 0x7c00

    ;清屏
    mov ax, 0600h
    mov bx, 0700h
    mov cx, 0
    mov dx, 184fh
    int 10h

    cli ;关中断

    ;写入中断向量表

    ;定时器
    mov word [0x8*4], clock_handler ;修改中断，跳转clock_handler处理中断;1c中断会被屏蔽，最好不要用
    mov word [0x8*4+2], 0
    mov al, 00110110b ; 方式3，用于定时产生中断
    out 0x43, al
    ; 计算计数值，产生20毫秒的时钟中断
    mov ax, 23863
    ; 将计数值分为低字节和高字节，发送到计数器0的数据端口（端口0x40）
    out 0x40, al        ; 低字节
    mov al, ah
    out 0x40, al        ; 高字节
    xor cx, cx
    mov bp, sp
    
    ;write
    mov word [0x80*4],write_handler
    mov word [0x80*4+2],0

    ;sleep
    mov word [0x81*4],sleep_handler
    mov word [0x81*4+2],0

    sti ;开中断

    ;加载应用程序 用call而不是jmp,因为有ret语句,通过助教推荐的方式实现，还可以通过切换堆栈iret来实现
    ;由于我已经在跳转回去app处用了iret实现，所以已经会了iret，此处就不用了
    call load_application

write_handler:
    mov bp,bx ; es:bp = 串地址
    ;设置打印格式
    mov ax, 01301h
    mov bx, 000Fh
    mov dh,[counter2]   ;行号，打一次加一次，如果用换行的话会错开，很丑
    mov dl,25  ;列号
    int 10h
    inc word [counter2]
    iret

clock_handler:
    inc word [counter]; 增加一次counter的数值
    
    mov dx, [num]
    mov cx, [counter]
    cmp cx, dx
    ;到点就跳转回去
    je back_to_main

    ;告知中断完成，允许产生新中断
    mov al,20h
    out 20h,al
    iret

loop:   ;归一化1s
    mov dx, [normalize]
    add ax, dx
    dec cx
    cmp cx, 0
    je continue
    jmp loop

sleep_handler:
    cli
    mov cx, 50  ;20ms*50=1s
    mov [normalize], ax
    jmp loop

continue:
    mov [num], ax
    sti ;一定要开中断，否则时钟中断被屏蔽，无法产生

    ;保护现场cx,dx
    push cx
    push dx
    push bp
    mov bp,sp;  bp此时就是回去的栈指针
    mov sp,idle_stack ;切换到idle栈
    ;   通过iret跳转idle进程执行
    pushf
    push word 0 ;cs
    push word idle; 
    iret


;idle进程，除非要跳转回main函数，否则一直死循环
idle:
    jmp idle

;通过sp切换进程回主函数
back_to_main:
    ;计数器清零
    mov word [counter], 0

    ;处理输出格式
    mov cx, [counter3]
    cmp cx, 19  ;每到20行清屏从第一行开始重新输出
    jne back
    
    ;清屏
    mov ax, 0600h
    mov bx, 0700h
    mov cx, 0
    mov dx, 184fh
    int 10h
    mov word [counter3], 0
    mov word [counter2], 0  ;从头输出

back:
    ;不好的实现：通过堆栈的方式jmp回main函数，已经舍去(与api中不好的实现对应，配合使用)
    ;mov sp, 0x1000-4
    ;pop cx
    ;mov sp, cx
    ;pop cx
    ;jmp cx

    ;好的实现，通过iret回main函数
    inc word [counter3]
    mov sp,bp  ;将堆栈指针设置为基址指针（Base Pointer，BP）的值，实际上是将堆栈指针重新指向了应用程序的栈。
    pop bp  ;弹出堆栈中的值，将其存入基址指针（BP）寄存器，这样恢复了应用程序的堆栈帧。
    pop dx ;恢复现场
    pop cx ;恢复现场
    mov al,20h
    out 20h,al
    iret  ;通过iret回到app

;加载应用程序
load_application:
    xor ax,ax
    mov es,ax 
    mov bx,0x8000;  link.ld中定义text在0x8000
    mov ah, 2    
    mov al, 1      
    mov ch, 0      
    mov cl, 2          
    mov dh, 0          
    mov dl, 0x80    
    int 0x13           
    mov sp,0x8000 ;切换到应用栈
    ;   通过iret跳转应用进程执行
    pushf
    push word 0 ;cs
    push word 0x8000; 
    iret

idle_stack equ 0x1000 ;idle栈
num dw 100;定义要打印的时间
normalize dw 50;归一化1s
counter dw 15;定义数据段 ;定义成0对于我的代码会有问题
counter2 dw 0 ; write行号
counter3 dw 0  ; 屏幕打到底下会显示不全，这时候清屏一下

times 510-($-$$) db 0 
dw 0xAA55