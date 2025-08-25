#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t rw_mutex;     // 控制对共享资源的访问
sem_t mutex_reader; // 保护 read_count 的互斥锁
sem_t mutex_writer; // 保护 write_count 的互斥锁
sem_t token;        // 控制访问顺序的令牌
int read_count = 0;
int write_count = 0;

void initialize_rwlock() {
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex_reader, 0, 1);
    sem_init(&mutex_writer, 0, 1);
    sem_init(&token, 0, 1); // 初始化令牌，初始值为1
}

void reader_lock() {
    sem_wait(&token); // 获取令牌
    sem_wait(&mutex_reader);
    read_count++;
    if (read_count == 1) {
        sem_wait(&rw_mutex);
    }
    sem_post(&mutex_reader);
    sem_post(&token); // 释放令牌
}

void reader_unlock() {
    sem_wait(&mutex_reader);
    read_count--;
    if (read_count == 0) {
        sem_post(&rw_mutex);
    }
    sem_post(&mutex_reader);
}

void writer_lock() {
    sem_wait(&token); // 获取令牌
    sem_wait(&rw_mutex);
}

void writer_unlock() {
    sem_post(&rw_mutex);
    sem_post(&token); // 释放令牌
}

void destroy_rwlock() {
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex_reader);
    sem_destroy(&mutex_writer);
    sem_destroy(&token);
}

// 测试用的读者和写者线程函数
void* reader(void* arg) {
    long id = (long)arg;
    reader_lock();
    printf("Reader %ld is reading...\n", id);
    sleep(1);  // 模拟读取操作
    printf("Reader %ld finished reading.\n", id);
    reader_unlock();
    return NULL;
}

void* writer(void* arg) {
    long id = (long)arg;
    writer_lock();
    printf("Writer %ld is writing...\n", id);
    sleep(1);  // 模拟写入操作
    printf("Writer %ld finished writing.\n", id);
    writer_unlock();
    return NULL;
}

int main() {
    // 测试自定义读写锁
    initialize_rwlock();
    
    pthread_t readers[5], writers[2];
    for (long i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, (void*)i);
        sleep(0.0001);
        if (i < 2) {
            pthread_create(&writers[i], NULL, writer, (void*)i);
            sleep(0.0001);
        }
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
        if (i < 2) {
            pthread_join(writers[i], NULL);
        }
    }
    
    destroy_rwlock();
}


// gcc -o PV_rwlock_my_fair PV_rwlock_my_fair.c -lpthread

// ./PV_rwlock_my_fair