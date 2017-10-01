/*
 @ 0xCCCCCCCC
*/

#include <atomic>
#include <iostream>
#include <thread>

#include <Windows.h>

std::atomic_flag g_ready;

void ThreadFunc()
{
    size_t count = 0;

    // Spin.
    while (g_ready.test_and_set(std::memory_order_acquire)) {
        ++count;
    }

    std::cout << "Thread spin count: " << count << "\n"
              << "Entering alertable state now!\n";

    auto rv = WaitForSingleObjectEx(GetCurrentProcess(), INFINITE, TRUE);
    std::cout << "Wait result: " << rv;
}

void CALLBACK APCProc(ULONG_PTR param)
{
    const char* const msg = reinterpret_cast<const char* const>(param);
    std::cout << "[Message] " << msg << "\n";
}

void TestAPC()
{
    g_ready.test_and_set(std::memory_order_acquire);

    std::thread th(ThreadFunc);

    const char* const msg1 = "first message";
    const char* const msg2 = "second message";
    const char* const msg3 = "third message";

    std::cout << "Start queuing APC\n";

    QueueUserAPC(APCProc, th.native_handle(), reinterpret_cast<ULONG_PTR>(msg1));
    QueueUserAPC(APCProc, th.native_handle(), reinterpret_cast<ULONG_PTR>(msg2));
    QueueUserAPC(APCProc, th.native_handle(), reinterpret_cast<ULONG_PTR>(msg3));

    g_ready.clear(std::memory_order_release);

    th.join();
}
