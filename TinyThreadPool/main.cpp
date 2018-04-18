/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include "tiny_thread_pool.h"

void Inc(int& i)
{
    ++i;
}

void PrintState(const char* msg, std::chrono::seconds delay)
{
    std::this_thread::sleep_for(delay);
    printf("%s\n", msg);
}

int main()
{
    using namespace std::chrono_literals;

    TinyThreadPool pool(2);

    pool.PostTask(&PrintState, "Prepare work 1", 2s);
    pool.PostTask(&PrintState, "Prepare work 2", 2s);

    pool.PostTaskWithShutdownBehavior(TaskShutdownBehavior::SkipOnShutdown, &PrintState, "Real work 1", 0s);
    pool.PostTaskWithShutdownBehavior(TaskShutdownBehavior::SkipOnShutdown, &PrintState, "Real work 2", 1s);

    pool.PostTask(&PrintState, "Real work 3", 2s);
    pool.PostTask(&PrintState, "Real work 4", 0s);

    pool.PostTaskWithShutdownBehavior(TaskShutdownBehavior::SkipOnShutdown, &PrintState, "Real work 5", 1s);

    return 0;
}
