/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include <Windows.h>

#include "kbase/command_line.h"
#include "kbase/error_exception_util.h"
#include <kbase/scoped_handle.h>

const wchar_t kProcessType[] = L"type";
const wchar_t kProcessChild[] = L"child";
const wchar_t kHandshake[] = L"handshake";

const wchar_t kSyncEventName[] = L"ipc_sync";

void LaunchChilld(HANDLE pipe_reader)
{
    std::cout << "Launching child...\n";

    HANDLE sync_event = CreateEventW(nullptr, FALSE, FALSE, kSyncEventName);

    kbase::CommandLine cmd_child(kbase::CommandLine::ForCurrentProcess().GetProgram());
    cmd_child.AppendSwitch(kProcessType, kProcessChild);
    cmd_child.AppendSwitch(kHandshake, std::to_wstring(reinterpret_cast<INT_PTR>(pipe_reader)));

    wchar_t launch_cmd[MAX_PATH] {0};
    wcscpy_s(launch_cmd, cmd_child.GetCommandLineString().c_str());

    STARTUPINFO startup_info {0};
    startup_info.cb = sizeof(startup_info);

    PROCESS_INFORMATION process_information {nullptr};

    BOOL rv = CreateProcessW(nullptr, launch_cmd, nullptr, nullptr, TRUE, CREATE_NEW_CONSOLE,
                             nullptr, nullptr, &startup_info, &process_information);
    ENSURE(CHECK, !!rv)(kbase::LastError()).Require();

    CloseHandle(process_information.hProcess);
    CloseHandle(process_information.hThread);

    std::cout << "Waiting for child to bind...\n";

    WaitForSingleObject(sync_event, INFINITE);
}

void RunAsParent()
{
    std::cout << "Parent is running...\n";

    HANDLE reader = nullptr, writer = nullptr;

    // We want the child process to inherit the reader handle.
    SECURITY_ATTRIBUTES sa {0};
    sa.bInheritHandle = TRUE;

    BOOL rv = CreatePipe(&reader, &writer, &sa, 0);
    ENSURE(CHECK, !!rv)(kbase::LastError()).Require();

    std::cout << "Anonymous pipe is created!\n";

    LaunchChilld(reader);

    std::cout << "Bind with child!\n";

    CloseHandle(reader);

    std::string msg;
    while (true) {
        std::getline(std::cin, msg, '\n');
        WriteFile(writer, msg.data(), static_cast<DWORD>(msg.size()), nullptr, nullptr);
        if (msg == "exit") {
            std::cout << "Bye!" << std::endl;
            return;
        }
    }
}

void RunAsChild()
{
    std::cout << "Child is running...\n";

    std::wstring handshake_value;
    kbase::CommandLine::ForCurrentProcess().GetSwitchValue(kHandshake, handshake_value);
    kbase::ScopedWinHandle pipe_reader(reinterpret_cast<HANDLE>(std::stoll(handshake_value)));

    kbase::ScopedWinHandle sync_event(OpenEventW(EVENT_ALL_ACCESS, FALSE, kSyncEventName));
    ENSURE(CHECK, !!sync_event)(kbase::LastError()).Require();
    SetEvent(sync_event.get());

    char buf[MAX_PATH] {0};
    while (true) {
        DWORD bytes_read;
        ReadFile(pipe_reader.get(), buf, MAX_PATH, &bytes_read, nullptr);
        std::cout.write(buf, bytes_read);
        std::cout << "\n";
        if (strncmp(buf, "exit", bytes_read) == 0) {
            std::cout << "Bye!" << std::endl;
            return;
        }
    }
}

void TestAnonymousPipe()
{
    kbase::CommandLine::Init(0, nullptr);

    std::wstring process_type;
    if (kbase::CommandLine::ForCurrentProcess().GetSwitchValue(kProcessType, process_type) &&
        process_type == kProcessChild) {
        RunAsChild();
    } else {
        RunAsParent();
    }
}
