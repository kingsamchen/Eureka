/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <thread>
#include <vector>

#include <windows.h>
#include <winsock2.h>

#include "kbase/at_exit_manager.h"
#include "kbase/command_line.h"
#include "kbase/error_exception_util.h"
#include "kbase/os_info.h"
#include "kbase/scope_guard.h"
#include "kbase/scoped_handle.h"

#include "iocp_utils.h"
#include "scoped_socket.h"
#include "tcp_connection_manager.h"
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

void InitializeWinsock()
{
    WSADATA data {0};
    auto result_code = WSAStartup(MAKEWORD(2, 2), &data);
    ENSURE(THROW, result_code == 0)(result_code).Require();
    std::cout << "-*- Windows Socket Library Initialized -*-\n";
}

void CleanWinsock()
{
    WSACleanup();
    std::cout << "-*- Windows Socket Library Cleaned -*-\n";
}

ScopedSocketHandle CreateListener(unsigned short port, int max_pending_clients)
{
    ScopedSocketHandle listener(
        WSASocketW(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED));
    ENSURE(CHECK, !!listener).Require();

    sockaddr_in server_addr {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int rv = bind(listener.get(), reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
    ENSURE(CHECK, rv == 0)(WSAGetLastError()).Require();

    rv = listen(listener.get(), max_pending_clients);
    ENSURE(CHECK, rv == 0)(WSAGetLastError()).Require();

    return listener;
}

std::vector<std::thread> LaunchWorkers(HANDLE io_port, SOCKET listener)
{
    auto worker_count = kbase::OSInfo::GetInstance()->number_of_cores() * 2;
    std::vector<std::thread> workers;
    for (size_t i = 0; i < worker_count; ++i) {
        workers.emplace_back(std::thread(Worker(io_port, listener)));
    }

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

    InitializeWinsock();
    ON_SCOPE_EXIT { CleanWinsock(); };

    SetConsoleCtrlHandler(ControlCtrlHandler, TRUE);
    ON_SCOPE_EXIT { SetConsoleCtrlHandler(nullptr, FALSE); };

    constexpr unsigned short kPort = 8088;
    constexpr int kMaxPending = 10;

    auto listener = CreateListener(kPort, kMaxPending);

    // Default to number of cores in the system.
    auto io_port = utils::CreateNewIOCP(0);
    ENSURE(CHECK, !!io_port)(kbase::LastError()).Require();

    bool success = utils::AssociateDeviceWithIOCP(reinterpret_cast<HANDLE>(listener.get()),
                                                  io_port.get(),
                                                  utils::CompletionKeyAccept);
    ENSURE(CHECK, success)(kbase::LastError()).Require();

    auto workers = LaunchWorkers(io_port.get(), listener.get());

    TcpConnectionManager::GetInstance()->ListenForNewClient();

    WaitForSingleObject(exit_event.get(), INFINITE);

    QuitWorkers(workers, io_port.get());

    return 0;
}
