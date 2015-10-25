/*
 @ 0xCCCCCCCC
*/

#include <future>
#include <iostream>
#include <random>

#include "count_down_latch.h"

const int kWorkerCount = 3;
std::random_device rd;
std::default_random_engine rde(rd());
std::uniform_int_distribution<> rn_gen(1, 3);

void DoWork(int work_id, CountDownLatch& start_signal, CountDownLatch& done_signal)
{
    std::cout << "worker[" << work_id << "] is waiting for start" << std::endl;
    start_signal.Wait();
    std::cout << "worker[" << work_id << "] starts working" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(rn_gen(rde)));
    std::cout << "worker[" << work_id << "] is done." << std::endl;
    done_signal.Countdown();
}

int main()
{
    CountDownLatch start_signal(1);
    CountDownLatch done_signal(kWorkerCount);

    std::cout << "main thread is preparing" << std::endl;
    {
        std::thread th1(DoWork, 1, std::ref(start_signal), std::ref(done_signal));
        std::thread th2(DoWork, 2, std::ref(start_signal), std::ref(done_signal));
        std::thread th3(DoWork, 3, std::ref(start_signal), std::ref(done_signal));

        std::cout << "main thread is doing sth" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "main thread signals workers" << std::endl;
        start_signal.Countdown();
        done_signal.Wait();

        // Make sure thread objects get destroyed before return from main.
        th1.join();
        th2.join();
        th3.join();
    }
    std::cout << "all workers have finished their job" << std::endl;

    return 0;
}