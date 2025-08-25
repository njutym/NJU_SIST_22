#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h> 

int main() {
    int pipefd[2];
    size_t total_written = 0;
    char buffer[4096];  
    memset(buffer, 'A', sizeof(buffer));

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 设置写端为非阻塞模式
    int flags = fcntl(pipefd[1], F_GETFL, 0);
    fcntl(pipefd[1], F_SETFL, flags | O_NONBLOCK);

    // 不断写入数据，直到失败
    while (1) {
        ssize_t bytes_written = write(pipefd[1], buffer, sizeof(buffer));
        if (bytes_written == -1) 
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) 
            {
                printf("Pipe is full after writing %zu bytes\n", total_written);
                break;
            } 
            else 
            {
                perror("write");
                close(pipefd[0]);
                close(pipefd[1]);
                exit(EXIT_FAILURE);
            }
        }
        total_written += bytes_written;
    }

    close(pipefd[0]);
    close(pipefd[1]);
    return 0;
}

// gcc max_pipe_size_test.c -o max_pipe_size_test
// ./max_pipe_size_test