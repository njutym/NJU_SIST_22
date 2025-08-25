#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// 自定义读者优先的读写锁
sem_t rw_mutex;  
sem_t mutex;     
int read_count = 0;

void initialize_rwlock() {
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);
}

void reader_lock() {
    sem_wait(&mutex);
    read_count++;
    if (read_count == 1) {
        sem_wait(&rw_mutex);
    }
    sem_post(&mutex);
}

void reader_unlock() {
    sem_wait(&mutex);
    read_count--;
    if (read_count == 0) {
        sem_post(&rw_mutex);
    }
    sem_post(&mutex);
}

void writer_lock() {
    sem_wait(&rw_mutex);
}

void writer_unlock() {
    sem_post(&rw_mutex);
}

void destroy_rwlock() {
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);
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

// gcc -o PV_rwlock_my_reader_first PV_rwlock_my_reader_first.c -lpthread

// ./PV_rwlock_my_reader_first