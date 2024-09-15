section .text

BITS 16
SECTION MBR vstart=0x7c00

;初始化，程序开始
_start:
    ;初始化栈，字节不够了，省一点
    xor ax, ax 
    ;mov ds, ax
    ;mov es, ax
    ;mov ss, ax
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
    mov sp,0x8000 ;切换到应用栈
    ;   通过iret跳转应用进程执行
    pushf
    push word 0
    push word 0x8000
    iret

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
        mov dh,[counter_line]
        mov dl,25  ;列号
        int 10h
        inc word [counter_line]
        cmp dh, 23
        je print_clear
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
    print_clear:
        call clear
        mov word [counter_line], 0
        iret
    
;sleep相关代码
    ;sleep中断
    sleep_handler:
        cli     ;此处关中断防止干扰，后面每一个分支都有唯一与之对应的sti关中断
        ;根据现在运行状态判断是谁调用了sleep
        cmp word [current_state], 1
        je father_sleep_handler
        cmp word [current_state], 0
        je child_sleep_handler
        iret
    
    ;父进程进入sleep的处理
    father_sleep_handler:   
        imul bx, 50             ;20ms*50=1s
        mov [father_counter], bx
        mov [father_stack], sp

        ;判断子进程是否可调度
        cmp word [is_child_sleeping], 1
        je father_change_to_idle
        jmp father_change_to_child

    ;子进程进入sleep的处理
    child_sleep_handler:
        imul bx, 50             ;20ms*50=1s
        mov [child_counter], bx
        mov [child_stack], sp

        ;判断父亲进程是否可调度
        cmp word [is_father_sleeping], 1
        je child_change_to_idle
        jmp child_change_to_father

;fork相关代码
    ;fork中断处理
    fork_handler:
        ;从当前sp将各数据复制到+0x1000的地址上
        mov si, sp
        mov di, sp
        add di, 0x1000
        mov cx, 0x300   ; 超过512个字节即可

        ;复制过程
        copy_loop:
            mov al, [si] ; 将源数据段中的一个字节加载到 AL 寄存器
            mov [di], al ; 将 AL 寄存器中的字节写入目标数据段
            inc si       ; 指向源数据段的下一个字节
            inc di       ; 指向目标数据段的下一个字节
            loop copy_loop ; 循环，直到 CX 寄存器计数为零

        ;复制过程中di改变了，需要重新初始化di成栈顶
        mov di,sp
        add di,0x1000
        mov [child_stack],di ;保存子进程栈指针
        
        ;修改子进程栈中fork的返回值ax,子进程返回0，short类型
        mov word [di+6], 0 

        ;修改fork的栈内容
        ;修改栈顶iret回去的IP
        mov ax,[di]
        add ax,0x1000
        mov [di],ax

        ;修改从fork系统调用返回main函数的地址
        mov ax,[di+8]
        add ax,0x1000
        mov [di+8],ax 

        ;fork返回,先返回父进程，在api中已经将父进程的ax保护为1
        ;而子进程ax返回0
        iret

;时钟中断相关代码
    ;时钟中断处理
    clock_handler:
        ;首先更新sp，通过current_state判断更新谁的
        mov cx, [current_state]
        cmp cx, 0 ;子进程
        je clock_child
        cmp cx, 1 ;父进程
        je clock_father
        cmp cx,2  ;idle进程
        je clock_idle

    ;由父进程进来的时钟中断
    clock_father:
        mov [father_stack],sp
        jmp handle_clock

    ;由子进程进来的时钟中断
    clock_child:
        mov [child_stack],sp
        jmp handle_clock

    ;由idle进程进来的时钟中断
    clock_idle:
        mov [idle_stack],sp
        jmp handle_clock
    
    ;再统一处理时钟中断
    handle_clock:
        ;这个函数的所有分支都会汇入judge_if_change

        ;首先计算以下进中断的时候是否有没在sleep的进程，如果有，cx会小于2，方便后面处理
        mov cx, [is_father_sleeping]
        mov dx, [is_child_sleeping]
        add cx, dx
        
        ;由于绝大部分时间都在sleep，为了省字节，不妨父进程和子进程计数器值都更新一下
        dec byte [father_counter]
        dec byte [child_counter]

        ;为了考虑极端情况，判断是否苏醒不用计数器是否为0，而用计数器是否减到小于1的值
        ;防止父进程子进程同时能唤醒，父进程先回去了，子进程计数器为-1而无法后续被唤醒

        ;判断父进程是否能够苏醒
        judge_father:
            cmp word [father_counter], 1
            jl father_awake

        ;判断子进程是否能够苏醒
        judge_child:
            cmp word [child_counter], 1
            jl child_awake
            jmp judge_if_change

    ;父进程苏醒
    father_awake:
        mov word [is_father_sleeping], 0
        jmp judge_child ;需要继续判断子进程能否苏醒
    
    ;子进程苏醒
    child_awake:
        mov word [is_child_sleeping], 0
        jmp judge_if_change

    ;判断是否从时钟中断跳转到别处
    judge_if_change:
        ;cx小于2代表子进程或者父进程至少有一个在运行,不做跳转
        cmp cx, 2
        jl int_exit 

        ;优先苏醒父进程
        cmp word [is_father_sleeping], 0
        je clock_change_to_father ;

        ;其次苏醒子进程
        cmp word [is_child_sleeping], 0
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
            mov word [current_state], 0
            mov [father_stack], sp
            mov sp, [child_stack]
            mov word [is_father_sleeping], 1
            sti   ;这里的cli与sleep_handler中的cli相互对应
            jmp int_exit

        child_change_to_father:
            mov word [current_state], 1 
            mov word [is_father_sleeping], 0    
            mov [child_stack],sp
            mov sp,[father_stack]
            mov word [is_child_sleeping], 1
            sti     ;这里的cli与sleep_handler中的cli相互对应
            jmp int_exit

        father_change_to_idle:
            mov word [is_father_sleeping], 1
            jmp change_to_idle

        child_change_to_idle:
            mov word [is_child_sleeping], 1
            jmp change_to_idle
            
        change_to_idle:
            sti    ;这里的cli与sleep_handler中的cli相互对应
            mov sp,[idle_stack] ;切换到idle栈
            mov word [current_state],2  ;idle进程将上台
            ;   通过iret跳转idle进程执行
            pushf
            push word 0 ;cs
            push word idle; ip
            iret

    ;clock中的跳转
    ;sp的值已经在刚进入时钟中断的时候更新给对应的进程了
        clock_change_to_father:
            mov word [current_state], 1
            mov sp, [father_stack]
            jmp int_exit

        clock_change_to_child:
            mov word [current_state], 0
            mov sp, [child_stack]
            jmp int_exit

;数据段
idle_stack dw 0x1000 ;idle栈
father_stack dw 0x8000 ;父进程栈
child_stack dw 0x9000 ;子进程栈

is_father_sleeping dw 0 ;父进程是否睡眠, 0--否，1--是
is_child_sleeping dw 0 ;子进程是否睡眠, 0--否，1--是

father_counter dw 0;定义父进程要打印的时间
child_counter dw 0;定义子进程要打印的时间

current_state dw 1;需要初始化为父进程 0--子进程，1--父进程，2--idle进程

counter_line dw 0 ; write行号

times 510-($-$$) db 0 
dw 0xAA55
