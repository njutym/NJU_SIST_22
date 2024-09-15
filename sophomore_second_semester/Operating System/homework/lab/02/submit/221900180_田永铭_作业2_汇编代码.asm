; 设置代码段的起始位置
BITS 16
SECTION MBR vstart=0x7c00

; 初始化堆栈(只安装了windows系统的mbr中最前面就是这些指令)
mov ax, cs
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7c00

_start:
    ; 清屏
    mov ax, 0x600
    mov bx, 0x700
    mov cx, 0
    mov dx, 0x184f
    int 10h

    ; 打印引导标签
    mov si, lead_label
    call print
    mov si, windows_label
    call print
    mov si, linux_label
    call print

    ; 输入
    call input

    ; 重新显示标签，也可以写成jmp $原地等待
    jmp _start

_exit:
    ret

; 打印字符串
print:
    lodsb
    or al, al
    jz _exit
    mov ah, 0x0E
    int 10h
    jmp print

; 用户输入
input:
    mov ah, 0
    int 16h
    cmp ah, 0
    je input

    cmp al, '1'
    je select_windows   ;想选windows
    cmp al, '2'
    je select_linux     ;想选linux
    jmp input

; 选择 Windows 分区
select_windows:
    ; 清屏
    mov ax, 0x600
    mov bx, 0x700
    mov cx, 0
    mov dx, 184fh
    int 10h
    ; 等enter
    mov byte [selected_partition], '1'
    mov si, windows_label
    call print
    jmp wait_enter

; 选择 Linux 分区
select_linux:
    ; 清屏
    mov ax, 0x600
    mov bx, 0x700
    mov cx, 0
    mov dx, 184fh
    int 10h
    ; 等enter
    mov byte [selected_partition], '2'
    mov si, linux_label
    call print
    jmp wait_enter

; 进入 Windows
windows_selected:
    ;基本照着只装了windows的原始的MBR来写就行

    ; 将MBR从0x7c00移到0x600
    xor ax, ax              ; 清零ax寄存器
    mov ss, ax              ; 将ax的值移动到ss寄存器
    mov sp, 0x7c00         ; 将栈指针sp设置为0x7c00
    mov es, ax              ; 将ax的值移动到es寄存器
    mov ds, ax              ; 将ax的值移动到ds寄存器
    mov si, 0x7c00         ; 将源地址0x7c00移动到si寄存器
    mov di, 0x600          ; 将目的地址0x600移动到di寄存器
    mov cx, 0x200          ; 将数据大小0x200移动到cx寄存器
    cld                     ; 清除方向位
    rep movsb               ; 重复执行movsb指令，将数据从si复制到di

    ; 这步需要注意将push word后的地址找出来，不是原本的地址，
    ; 而是这个新写的MBR汇编成的二进制代码再反汇编生成对应的顺序地址
    ; (加上0x600偏移量)
    push ax                 ; 将ax寄存器的值入栈
    push word 0x6A3         ; 将0x6A3入栈
    retf                    ; 返回前先弹出标志寄存器，然后根据指定的代码段和偏移地址进行跳转

    ; 我的Windows的加载器在第一个分区的活动扇区0x7be
    ; 使用CHS法
    mov bp, 0x7be           ; 将0x7be移动到bp寄存器，该地址指向活动标志的位置
    mov ax, 0x0201          ; 将0x0201移动到ax寄存器，指定扇区数和扇区号
    mov bx, 0x7c00          ; 将0x7c00移动到bx寄存器，指定数据的加载地址
    mov dl, [bp+0x0]        ; 将bp寄存器偏移0x0处的值移动到dl寄存器，指定驱动器号
    mov dh, [bp+0x1]        ; 将bp寄存器偏移0x1处的值移动到dh寄存器，指定磁头号
    mov cl, [bp+0x2]        ; 将bp寄存器偏移0x2处的值移动到cl寄存器，指定扇区号的低8位
    mov ch, [bp+0x3]        ; 将bp寄存器偏移0x3处的值移动到ch寄存器，指定扇区号的高2位
    int 0x13                ; 调用int 0x13中断
    jmp 0x0000:0x7c00       ; 跳转到0x7c00地址

; 进入 Linux
linux_selected:
    ;大多数指令来自原汇编代码
    
    mov dl, 0x80   ; 将驱动器号设为0x80

    ; 使用LBA法，读到0x7000
    xor ax, ax           ; 清零ax寄存器
    mov [si+0x4], ax     ; 将0写入si+0x4处，即设定LBA高字节为0
    inc ax               ; ax加1，此时ax=1
    mov [si-0x1], al     ; 将al的值写入si-0x1处，即设定LBA低字节为1
    mov [si+0x2], ax     ; 将ax的值写入si+0x2处，即设定LBA的低16位为1
    mov word [si], 0x10 ; 将0x10写入si处，即设置读取的扇区数量为16
    mov ebx, 0x00000001 ; 将1写入ebx寄存器
    mov [si+0x8], ebx    ; 将ebx的值写入si+0x8处，即设定读取的磁头号为1
    mov ebx, 0x00000000 ; 将0写入ebx寄存器
    mov [si+0xc], ebx    ; 将ebx的值写入si+0xc处，即设定读取的柱面号为0
    mov word [si+0x6], 0x7000 ; 将0x7000写入si+0x6处，即设置读取的起始扇区号为0x7000
    mov ah, 0x42         ; 将0x42写入ah寄存器，即设定读取扇区的子功能号
    int 0x13             ; 调用0x13中断

    ; 将start从0x7000移动到0x8000启动
    mov bx, 0x7000       ; 将0x7000写入bx寄存器
    mov es, bx           ; 将bx的值写入es寄存器，即将es寄存器设定为0x7000
    xor bx, bx           ; 将bx寄存器清零
    mov ax, 0x201        ; 将0x201写入ax寄存器，指定读取1个扇区到es:bx
    int 0x13             ; 调用0x13中断
    mov bx, es           ; 将es的值写入bx寄存器
    pusha                ; 将通用寄存器入栈
    push ds              ; 将ds寄存器入栈
    mov cx, 0x100        ; 将0x100写入cx寄存器，指定移动的字节数
    mov ds, bx           ; 将bx的值写入ds寄存器
    xor si, si           ; 将si寄存器清零
    mov di, 0x8000       ; 将0x8000写入di寄存器，即设置目的地址为0x8000
    mov es, si           ; 将si的值写入es寄存器，即将es寄存器设定为0x8000
    cld                  ; 清除方向位，确保字符串传送操作为正向传送
    rep movsw            ; 重复执行movsw指令，将ds:si指向的字符串复制
                        ;es:di指向的位置
    pop ds               ; 弹出ds寄存器的值
    popa                 ; 弹出通用寄存器的值
    jmp 0x8000           ; 跳转到0x8000
    
; 等待用户按下 Enter 键
wait_enter:
    mov ah, 0
    int 16h
    cmp al, 0x0D    ; Enter 键的 ASCII 码为 0x0D
    je execute_selected_partition
    ;使用户可以切换1，2
    cmp al,'1'  
    je select_windows
    cmp al,'2'
    je select_linux
    jmp wait_enter

; 执行用户选择的分区
execute_selected_partition:
    cmp byte [selected_partition], '1'
    je windows_selected
    cmp byte [selected_partition], '2'
    je linux_selected
    jmp _start

; 定义字符串
lead_label db "press 1 for windows, 2 for linux", 0x0D,0x0A,0
windows_label db "1. windows ", 0x0D,0x0A,0
linux_label   db "2. linux ", 0x0D,0x0A,0
selected_partition db 0 ;可在这里修改默认选择的分区

; MBR 结束标志
times 510-($-$$) db 0
dw 0xAA55