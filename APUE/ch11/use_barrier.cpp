/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <random>

#include <pthread.h>

int g_seq[2];

pthread_barrier_t barrier;

void* ThreadFunc(void* arg)
{
    std::random_device rd;
    auto seq = rd();
    auto idx = reinterpret_cast<long>(arg);
    
    std::cout << "thread " << idx << " is done and ready to wait\n";
    pthread_barrier_wait(&barrier);

    g_seq[idx] = seq;

    return reinterpret_cast<void*>(0);
}

int main()
{
    pthread_barrier_init(&barrier, nullptr, 2);
    
    constexpr int kThreadNum = 2;
    pthread_t threads[kThreadNum];
    
    for (int i = 0; i < kThreadNum; ++i) {
        pthread_create(&threads[i], nullptr, ThreadFunc, reinterpret_cast<void*>(i));
    }

    for (int i = 0; i < kThreadNum; ++i) {
        pthread_join(threads[i], nullptr);
    }

    std::cout << "Generated seq are: " << g_seq[0] << "\t" << g_seq[1] << "\n";
    
    pthread_barrier_destroy(&barrier);

    return 0;
}

