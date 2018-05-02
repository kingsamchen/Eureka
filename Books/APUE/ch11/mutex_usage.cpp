/*
 @ 0xCCCCCCCC
*/

#include <cstdio>

#include <unistd.h>
#include <pthread.h>

int g_count = 0;

pthread_mutex_t g_count_mutex;

void* ThreadFunc(void* arg)
{
    printf("Thread %ld is running!\n", reinterpret_cast<size_t>(arg));

    pthread_mutex_lock(&g_count_mutex);
    for (int i = 1; i <= 100; ++i) {
        ++g_count;
    }
    pthread_mutex_unlock(&g_count_mutex);

    return static_cast<void*>(0);
}

int main()
{
    pthread_mutex_init(&g_count_mutex, nullptr);

    constexpr size_t kThreadNum = 3;
    pthread_t threads[kThreadNum];
    for (size_t i = 0; i < kThreadNum; ++i) {
        auto err = pthread_create(&threads[i], nullptr, ThreadFunc, reinterpret_cast<void*>(i));
        if (err != 0) {
            fputs("creating thread failed", stderr);
            _exit(1);
        }
    }

    for (size_t i = 0; i < kThreadNum; ++i) {
        printf("Reap resource for thread %ld\n", i);
        pthread_join(threads[i], nullptr);
    }

    pthread_mutex_destroy(&g_count_mutex);

    printf("value of global counter %d\n", g_count);

    return 0;
}
