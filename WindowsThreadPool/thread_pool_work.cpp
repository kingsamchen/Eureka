/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include <Windows.h>
#include <threadpoolapiset.h>

void CALLBACK WorkCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK work)
{
    std::cout << "[*] Doing work on " << GetCurrentThreadId() << " !\n";
    Sleep(2000);
    std::cout << "[*] Work is done on " << GetCurrentThreadId() << " !\n";
}

void TestThreadpoolWork()
{
    // However, context is associated with since the work-pool being created, which limits
    // the usage of the context in callback.
    auto work_id = CreateThreadpoolWork(WorkCallback, nullptr, nullptr);
    if (!work_id) {
        std::cerr << "CreateThreadpoolWork failed!\n";
        return;
    }

    std::cout << "[M] Work pool is created!\n";

    Sleep(1000);

    std::cout << "[M] Preparing dispatching works!\n";

    for (int i = 0; i < 8; ++i) {
        SubmitThreadpoolWork(work_id);
    }

    std::cout << "[M] All works are dispatched!\n";

    WaitForThreadpoolWorkCallbacks(work_id, FALSE);

    CloseThreadpoolWork(work_id);
}
