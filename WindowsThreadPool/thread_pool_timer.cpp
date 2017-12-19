/*
 @ 0xCCCCCCCC
*/

#include <cassert>
#include <iostream>

#include <Windows.h>

constexpr size_t kMaxCount = 5;
HANDLE g_event = nullptr;

void CALLBACK TimerCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_TIMER timer)
{
    auto& count = *static_cast<size_t*>(context);
    ++count;

    std::cout << count << "th timer expired!\n";

    if (count >= 5) {
        SetThreadpoolTimer(timer, nullptr, 0, 0);
        SetEvent(g_event);
    }
}

void TestThreadpoolTimer()
{
    size_t fired_count = 0;
    auto timer_id = CreateThreadpoolTimer(TimerCallback, &fired_count, nullptr);
    if (!timer_id) {
        std::cerr << "Create timer thread pool failed!\n";
        return;
    }

    std::cout << "Timer thread pool is created!\n";

    ULARGE_INTEGER uli {0};
    uli.QuadPart = - (10 * 1000 * 1000 * static_cast<int64_t>(5));
    FILETIME first_time {0};
    first_time.dwLowDateTime = uli.LowPart;
    first_time.dwHighDateTime = uli.HighPart;
    SetThreadpoolTimer(timer_id, &first_time, 1000, 0);

    std::cout << "Timer is setup!";

    g_event = CreateEventW(nullptr, FALSE, FALSE, nullptr);

    WaitForSingleObject(g_event, INFINITE);

    // WaitForThreadpoolTimerCallbacks works somewhat confusing.
    WaitForThreadpoolTimerCallbacks(timer_id, FALSE);
    CloseThreadpoolTimer(timer_id);
}
