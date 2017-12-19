/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include <Windows.h>

void CALLBACK WaitCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WAIT wait,
                           TP_WAIT_RESULT result)
{
    std::cout << "WaitCallback is called!" << std::endl;
}

void TestThreadpoolWait()
{
    auto wait_id = CreateThreadpoolWait(WaitCallback, nullptr, nullptr);
    HANDLE h_e = CreateEventW(nullptr, FALSE, FALSE, nullptr);

    std::cout << "Wait thread pool and event are created" << std::endl;

    SetThreadpoolWait(wait_id, h_e, nullptr);

    std::cout << "Bind wait pool with event" << std::endl;

    Sleep(5000);

    std::cout << "Signaling event" << std::endl;

    SetEvent(h_e);

    WaitForThreadpoolWaitCallbacks(wait_id, FALSE);
    CloseThreadpoolWait(wait_id);

    std::cout << "All DONE\n";
}
