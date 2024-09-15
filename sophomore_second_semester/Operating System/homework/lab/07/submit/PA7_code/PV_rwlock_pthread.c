#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// 使用pthread库提供的读写锁定义
pthread_rwlock_t rwlock;

void* reader_pthread(void* arg) {
    pthread_rwlock_rdlock(&rwlock);
    printf("Pthread Reader %ld is reading...\n", (long)arg);
    sleep(1);
    printf("Pthread Reader %ld finished reading.\n", (long)arg);
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

void* writer_pthread(void* arg) {
    pthread_rwlock_wrlock(&rwlock);
    printf("Pthread Writer %ld is writing...\n", (long)arg);
    sleep(1);
    printf("Pthread Writer %ld finished writing.\n", (long)arg);
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

int main() {
    
    // 测试pthread库的读写锁
    pthread_rwlock_init(&rwlock, NULL);
    
    pthread_t readers[5], writers[2];

    for (long i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader_pthread, (void*)i);
        sleep(0.0001);
        if (i < 2) {
            pthread_create(&writers[i], NULL, writer_pthread, (void*)i);
            sleep(0.0001);
        }
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
        if (i < 2) {
            pthread_join(writers[i], NULL);
        }
    }
    
    pthread_rwlock_destroy(&rwlock);

    return 0;
}


/*
gcc -o PV_rwlock_pthread PV_rwlock_pthread.c -lpthread
./PV_rwlock_pthread
*/