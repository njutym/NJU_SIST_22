#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    // 创建管道 fd[0]-读端，fd[1]-写端
    int fd[2];
    pid_t cat_id, sort_id;
    pipe(fd);

    // 第一次创建子进程，用以cat
    cat_id = fork();
    if(cat_id == 0)
    {
        close(fd[0]); // 关闭管道的读端，关闭不需要的管道读端有助于避免资源泄漏。

        dup2(fd[1], STDOUT_FILENO); // 将标准输出重定向到管道的写端
    
        close(fd[1]); // 关闭管道的写端，关闭 fd[1] 有助于防止意外的文件描述符泄漏，并确保子进程结束时，写端被正确关闭，使得读端能够检测到EOF（文件结束）。
        execlp("cat", "cat", "test1.txt", "test2.txt", (char *) NULL);
    }

    // 第二次创建子进程，用以sort
    sort_id = fork();
    if (sort_id == 0) 
    { 
        close(fd[1]);// 关闭管道的写端

        dup2(fd[0], STDIN_FILENO);// 将标准输入重定向到管道的读端

        close(fd[0]);// 关闭管道的读端

        execlp("sort", "sort", (char *) NULL);
    }

    // 关闭父进程中的管道文件描述符
    close(fd[0]);
    close(fd[1]);

    // 等待子进程结束
    waitpid(cat_id, NULL, 0);
    waitpid(sort_id, NULL, 0);

    return 0;
}

// gcc input_output_redirection.c -o input_output_redirection
// ./input_output_redirection