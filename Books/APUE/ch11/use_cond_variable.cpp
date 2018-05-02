/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <queue>
#include <random>

#include <pthread.h>
#include <unistd.h>

std::queue<int> data_queue;

// We make sure the consuming is always faster than producing.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void* Producer(void* arg)
{
    std::random_device rd;
    while (true) {
        int data = rd();
        std::cout << "Produce data -> " << data << "\n";
        pthread_mutex_lock(&mutex);
        data_queue.push(data);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&not_empty);
        sleep(1);
    }
}

void* Consumer(void* arg)
{
    while (true) {
        pthread_mutex_lock(&mutex);

        while (data_queue.empty()) {
            std::cout << "Data queue is empty now...\n";
            pthread_cond_wait(&not_empty, &mutex);
        }

        int data = data_queue.back();
        data_queue.pop();

        pthread_mutex_unlock(&mutex);

        std::cout << "Consume data -> " << data << "\n";
        sleep(1);
    }
}

int main()
{
    pthread_t producer, consumer;
    pthread_create(&producer, nullptr, Producer, nullptr);
    pthread_create(&consumer, nullptr, Consumer, nullptr);

    pthread_join(producer, nullptr);
    pthread_join(consumer, nullptr);

    return 0;
}
