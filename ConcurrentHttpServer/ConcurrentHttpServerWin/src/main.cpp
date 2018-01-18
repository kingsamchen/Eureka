/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include <windows.h>
#include <winsock2.h>

#include "kbase/at_exit_manager.h"
#include "kbase/command_line.h"
#include "kbase/error_exception_util.h"
#include "kbase/os_info.h"
#include "kbase/scope_guard.h"
#include "kbase/scoped_handle.h"

#include "iocp_utils.h"

namespace {

BOOL WINAPI ControlCtrlHandler(DWORD ctrl)
{
    switch (ctrl) {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
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

}   // namespace

int main()
{
    kbase::CommandLine::Init(0, nullptr);
    kbase::AtExitManager exit_manager;

    InitializeWinsock();
    ON_SCOPE_EXIT { CleanWinsock(); };

    SetConsoleCtrlHandler(ControlCtrlHandler, TRUE);
    ON_SCOPE_EXIT { SetConsoleCtrlHandler(nullptr, FALSE); };

    DWORD kConcurrentWorkers = kbase::OSInfo::GetInstance()->number_of_cores();
    auto io_port = utils::CreateNewIOCP(utils::CompletionKeyIO, kConcurrentWorkers);

    std::cin.get();

    return 0;
}
