/*
 @ 0xCCCCCCCC
*/

#include <stdio.h>

#include <thread>
#include <vector>

#include <windows.h>

#include "kbase/at_exit_manager.h"
#include "kbase/command_line.h"
#include "kbase/error_exception_util.h"
#include "kbase/os_info.h"
#include "kbase/scope_guard.h"
#include "kbase/scoped_handle.h"

#include "iocp_utils.h"
#include "tcp_connection_manager.h"
#include "winsock_ctx.h"
#include "worker.h"

namespace {

kbase::ScopedWinHandle exit_event(CreateEventW(nullptr, FALSE, FALSE, nullptr));

BOOL WINAPI ControlCtrlHandler(DWORD ctrl)
{
    switch (ctrl) {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
            SetEvent(exit_event.get());
            return TRUE;

        default:
            return FALSE;
    }
}

std::vector<std::thread> LaunchWorkers(HANDLE io_port)
{
    auto worker_count = kbase::OSInfo::GetInstance()->number_of_cores() * 2;
    std::vector<std::thread> workers;
    for (size_t i = 0; i < worker_count; ++i) {
        workers.emplace_back(std::thread(Worker(io_port)));
    }

    printf("%u workers launched!\n", worker_count);

    return workers;
}

void QuitWorkers(std::vector<std::thread>& workers, HANDLE io_port)
{
    for (size_t i = 0; i < workers.size(); ++i) {
        PostQueuedCompletionStatus(io_port, 0, utils::CompletionKeyShutdown, nullptr);
    }

    std::for_each(workers.begin(), workers.end(), std::mem_fn(&std::thread::join));
}

}   // namespace

int main()
{
    kbase::CommandLine::Init(0, nullptr);
    kbase::AtExitManager exit_manager;

    winsock_ctx::Init();
    ON_SCOPE_EXIT { winsock_ctx::Cleanup(); };

    SetConsoleCtrlHandler(ControlCtrlHandler, TRUE);
    ON_SCOPE_EXIT { SetConsoleCtrlHandler(nullptr, FALSE); };

    constexpr unsigned short kPort = 8088;

    TcpConnectionManager::GetInstance()->Configure(kPort, 0);

    printf("Connection manager has been configured!\n");

    auto io_port = TcpConnectionManager::GetInstance()->io_port();

    auto workers = LaunchWorkers(io_port);

    TcpConnectionManager::GetInstance()->ListenForClient();

    printf("Connection manager is listening on localhost:%d\n", kPort);

    WaitForSingleObject(exit_event.get(), INFINITE);

    QuitWorkers(workers, io_port);

    return 0;
}
