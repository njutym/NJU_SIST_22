#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

#define TOTAL_POINTS 10080000 // 2*3*2*5*7*2*3 * 4000 取了一个合适大小的数字，同时能被1到10以及16、32、64整除
#define MAX_THREAD_NUMS 64  // 最大允许的线程数

int thread_nums = 1; // 进行实验的线程数
int points_in_circle_nums = 0; //落入圆内的总点数
int nums_of_points_to_sample_of_each_thread; //每个线程需要采样的总点数
double pi = 0; //pi的计算值
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 定义互斥锁

// 自定义实现的随机数，相比于gcc实现没有加互斥锁，每个线程各自用各自随机数，不再因为加锁而损失效率
// unsigned int myrand(unsigned int *seed)
// {
//     return (*seed) = ((((*seed) * 1103515245) + 12345) & 0xffffffff);
// }

//采样函数
void *sampling(void *arg)
{
    //获取线程id
    int *id = (int *)arg;
    //统计该线程计算得到的落入圆内的点数
    int cnt = 0;
    // srand((unsigned int)time(NULL)); 
    unsigned int seed = (unsigned int)time(NULL); //定义时间种子
    
    //一共采样nums_of_points_to_sample_of_each_thread个
    for (int i = 0; i < nums_of_points_to_sample_of_each_thread; i++)
    {
        // 一定不能用rand，应该用rand_r！！！
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        // 下面是自定义实现的rand，也可以用
        // double x = (double)myrand(&seed) / 0xffffffff;
        // double y = (double)myrand(&seed) / 0xffffffff;
        if (x * x + y * y <= 1) cnt++; //落在圆内，则cnt++
    }
    //由于多线程要对全局变量points_in_circle_nums修改，要上互斥锁保护一下
    pthread_mutex_lock(&mutex);
    points_in_circle_nums += cnt;
    pthread_mutex_unlock(&mutex);
    return (void *)0;
}

// 主函数
int main(int argc, char *argv[])
{
    //如果没使用-t传参
    if(argc == 1) thread_nums = 1;
    //如果使用了-t传参
    else if (argc != 3 || argv[1][0] != '-' || argv[1][1] != 't')
    {
        printf("grammer fault!\n");
        return 1;
    }
    else
    {
        if (thread_nums = atoi(argv[2])) {}
        else
        {
            printf("argument invalid!\n");
            return 1;
        }
    }
    
    printf("线程数目: %d\n", thread_nums);
    //求线程需要采样的数目
    nums_of_points_to_sample_of_each_thread = TOTAL_POINTS / thread_nums;

    //分配线程号，用于传参
    pthread_t threads[MAX_THREAD_NUMS];
    int threadsId[MAX_THREAD_NUMS];
    for (int i = 0; i < MAX_THREAD_NUMS; i++) threadsId[i] = i;
    
    //创线程
    for (int i = 0; i < thread_nums; i++) pthread_create(&threads[i], NULL, sampling, &threadsId[i]);

    //销线程
    for (int i = 0; i < thread_nums; i++) pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&mutex); // 销毁互斥锁

    //计算pi
    pi = 4.0 * points_in_circle_nums / TOTAL_POINTS;

    //打印结果
    printf("总点数的值: %d\n", TOTAL_POINTS);
    printf("落在圆内的总点数的值: %d\n", points_in_circle_nums);
    printf("计算得到的π的值: %lf\n", pi);
    return 0;
}