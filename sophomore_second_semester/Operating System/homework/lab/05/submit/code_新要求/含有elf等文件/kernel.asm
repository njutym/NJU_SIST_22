section .text

BITS 16
SECTION MBR vstart=0x7c00

;初始化，程序开始
_start:
    ;初始化栈
    xor ax, ax 
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x8000

    call clear

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
    
    ;write
    mov word [0x80*4],write_handler
    mov word [0x80*4+2],0

    ;sleep
    mov word [0x81*4],sleep_handler
    mov word [0x81*4+2],0

    ;fork
    mov word [0x82*4],fork_handler
    mov word [0x82*4+2],0

    sti ;开中断

    ;加载应用程序 通过iret来实现
    call load_application

;加载应用程序
load_application:
    mov bx,0x8000;  link.ld中定义text在0x8000
    mov ah, 2    
    mov al, 1      
    mov ch, 0      
    mov cl, 2          
    mov dh, 0          
    mov dl, 0x80    
    int 0x13           
    ;   通过iret跳转应用进程执行,字节不够了，先不管了，jmp一下
    ;pushf
    ;push word 0
    ;push word 0x8000
    ;iret
    jmp 0x8000

;idle进程
idle:
    jmp idle

;write相关代码
    ;write中断
    write_handler:
        mov bp,bx ; es:bp = 串地址
        ;设置打印格式
        mov ax, 01301h
        mov bx, 000Fh
        mov dh,[es:counter_line]
        mov dl,25  ;列号
        int 10h
        inc byte [es:counter_line]
        ; 字节不够了，真不够了，不换行了我，打印到屏幕最底下会出错
       ; cmp dh, 23
       ; je print_clear
        iret

    ;清屏
    clear:
        mov ax, 0600h
        mov bx, 0700h
        mov cx, 0
        mov dx, 184fh
        int 10h
        ret

    ;重置行号
   ; print_clear:
      ;  call clear
       ; mov byte [es:counter_line], 0
       ; iret
    
;sleep相关代码
    ;sleep中断
    sleep_handler:
        cli     ;此处关中断防止干扰，后面每一个分支都有唯一与之对应的sti关中断
        imul bx, 50             ;20ms*50=1s
        ;根据现在运行状态判断是谁调用了sleep
        cmp byte [es:current_state], 1
        je father_sleep_handler
        cmp byte [es:current_state], 0
        je child_sleep_handler
        iret
    
    ;父进程进入sleep的处理
    father_sleep_handler:   
        mov [es:father_counter], bx
        mov [es:father_stack], sp

        ;判断子进程是否可调度
        cmp byte [es:is_child_sleeping], 1
        je father_change_to_idle
        jmp father_change_to_child

    ;子进程进入sleep的处理
    child_sleep_handler:
        ;改变段地址回0x0000
        xor ax,ax
        mov ds,ax
        mov ss,ax
        mov [child_counter], bx
        mov [child_stack], sp

        ;判断父亲进程是否可调度
        cmp byte [is_father_sleeping], 1
        je child_change_to_idle
        jmp child_change_to_father

;fork相关代码
    ;fork中断处理,段地址从0x0000:0x8000到0x1000:0x8000
    fork_handler:
        mov [child_stack],sp ;保存子进程栈指针

        ;切到子进程段进行复制
        mov word es, [child_ds]

        mov di, sp
        mov si, sp
        mov cx, 0x300
        cld
        rep movsb

        ;es保存为父进程段，此后不再改变，可以被利用
        xor ax, ax
        mov es, ax
        
        ;修改子进程段地址的内容
        mov word ds, [child_ds]

        ;重新更新di指向sp
        mov di, sp
        ;修改子进程栈中fork的返回值ax,子进程返回0，short类型
        mov [di+6], ax

        ;修改回段地址为0x0000
        mov ds, ax

        ;fork返回,先返回父进程，在api中已经将父进程的ax保护为1
        ;而子进程ax返回0
        iret

;时钟中断相关代码
    ;时钟中断处理
    clock_handler:
        mov ds, [father_ds]
        ;首先更新sp，通过current_state判断更新谁的
        cmp byte [current_state], 0
        je clock_child
        cmp byte [current_state], 1
        je clock_father
        cmp byte [current_state], 2
        je clock_idle

    ;由父进程进来的时钟中断
    clock_father:
        mov [father_stack],sp
        jmp handle_clock

    ;由子进程进来的时钟中断
    clock_child:
        mov [child_stack],sp
        mov ds, [child_ds] ;子进程来，需要修改ds
        jmp handle_clock

    ;由idle进程进来的时钟中断
    clock_idle:
        mov [idle_stack],sp
        jmp handle_clock
    
    ;再统一处理时钟中断
    handle_clock:
        ;这个函数的所有分支都会汇入judge_if_change

        ;首先计算一下进中断的时候是否有没在sleep的进程
        cmp byte [es:is_father_sleeping], 0
        mov ds, [father_ds];恢复父进程es
        je int_exit
        
        cmp word [es:is_child_sleeping], 0
        je int_exit
        
        ;由于绝大部分时间都在sleep，为了省字节，不妨父进程和子进程计数器值都更新一下
        dec byte [father_counter]
        dec byte [child_counter]

        ;为了考虑极端情况，判断是否苏醒不用计数器是否为0，而用计数器是否减到小于1的值
        ;防止父进程子进程同时能唤醒，父进程先回去了，子进程计数器为-1而无法后续被唤醒

        ;判断父进程是否能够苏醒
        judge_father:
            cmp byte [father_counter], 1
            jl father_awake

        ;判断子进程是否能够苏醒
        judge_child:
            cmp byte [child_counter], 1
            jl child_awake
            jmp judge_if_change

    ;父进程苏醒
    father_awake:
        mov byte [is_father_sleeping], 0
        jmp judge_child ;需要继续判断子进程能否苏醒
    
    ;子进程苏醒
    child_awake:
        mov byte [is_child_sleeping], 0
        jmp judge_if_change

    ;判断是否从时钟中断跳转到别处
    judge_if_change:
        ;优先苏醒父进程
        cmp byte [is_father_sleeping], 0
        je clock_change_to_father ;

        ;其次苏醒子进程
        cmp byte [is_child_sleeping], 0
        je clock_change_to_child  ;
            
        jmp int_exit

    ;时钟中断返回(含EOF)
    int_exit:
        mov al,20h
        out 20h,al
        iret

;跳转进程相关代码
    ;sleep中的跳转
    ;注意修改是否在睡眠状态这一步要放在后面执行，防止时钟中断的干扰
        father_change_to_child:
            mov byte [current_state], 0
            mov [father_stack], sp
            mov sp, [child_stack]
            mov byte [is_father_sleeping], 1

            ;段地址换成子进程的
            mov ss, [child_ds]
            mov ds, [child_ds]
            sti   ;这里的cli与sleep_handler中的cli相互对应
            jmp int_exit

        child_change_to_father:
            mov byte [current_state], 1 
            mov byte [is_father_sleeping], 0    
            mov [child_stack],sp
            mov sp,[father_stack]
            mov byte [is_child_sleeping], 1

            sti     ;这里的cli与sleep_handler中的cli相互对应
            jmp int_exit

        father_change_to_idle:
            mov byte [is_father_sleeping], 1
            jmp change_to_idle

        child_change_to_idle:
            mov byte [is_child_sleeping], 1
            jmp change_to_idle
            
        change_to_idle:
            sti    ;这里的cli与sleep_handler中的cli相互对应
            mov sp,[idle_stack] ;切换到idle栈
            mov byte [current_state],2  ;idle进程将上台
            ;   通过iret跳转idle进程执行
            pushf
            push word 0 ;cs
            push word idle; ip
            iret

    ;clock中的跳转
    ;sp的值已经在刚进入时钟中断的时候更新给对应的进程了
        clock_change_to_father:
            mov byte [current_state], 1
            mov sp, [father_stack]
            jmp int_exit

        clock_change_to_child:
            mov byte [current_state], 0
            mov sp, [child_stack]

            ;段地址换成子进程的
            mov ss, [child_ds]
            mov ds, [child_ds]
            jmp int_exit


idle_stack dw 0x1000 ;idle栈
father_stack dw 0x8000 ;父进程栈
child_stack dw 0x9000 ;子进程栈

child_ds dw 0x1000 ;子进程所在段地址
father_ds dw 0x0000 ;父进程和idle以及内核所在段地址

is_father_sleeping db 0 ;父进程是否睡眠, 0--否，1--是s
is_child_sleeping db 0 ;子进程是否睡眠, 0--否，1--是

father_counter db 0;定义父进程要打印的时间
child_counter db 0;定义子进程要打印的时间

current_state db 1;需要初始化为父进程 0--子进程，1--父进程，2--idle进程

counter_line db 0 ; write行号

times 510-($-$$) db 0 
dw 0xAA55
