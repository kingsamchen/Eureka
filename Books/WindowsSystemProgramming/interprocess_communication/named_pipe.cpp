/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <thread>

#include <Windows.h>

#include "kbase/command_line.h"
#include "kbase/error_exception_util.h"
#include "kbase/scoped_handle.h"
#include "kbase/string_util.h"

const wchar_t kPipeName[] = LR"(\\.\pipe\wsp_test)";
constexpr DWORD kBufSize = 256;

const wchar_t kProcessType[] = L"type";
const wchar_t kProcessClient[] = L"client";

void WorkerFunc(kbase::ScopedWinHandle&& bound_pipe)
{
    kbase::ScopedWinHandle pipe(std::move(bound_pipe));

    ULONG client_pid = 0;
    GetNamedPipeClientProcessId(pipe.get(), &client_pid);
    std::cout << "Worker created for client " << client_pid << std::endl;

    char buf[kBufSize];

    while (true) {
        DWORD bytes_read = 0;
        BOOL success = ReadFile(pipe.get(), buf, kBufSize, &bytes_read, nullptr);
        if (!success || bytes_read == 0) {
            kbase::LastError last_error;
            if (last_error.error_code() == ERROR_BROKEN_PIPE) {
                std::cout << "Client " << client_pid << " disconnected" << std::endl;
            } else {
                std::cout << "Read pipe for client " << client_pid << "failed!" << last_error
                          << std::endl;
            }

            break;
        }

        std::string msg(buf, bytes_read);
        std::cout << "[Client " << client_pid << "]: " << msg << std::endl;

        kbase::ASCIIStringToUpper(msg);

        success = WriteFile(pipe.get(), msg.data(), static_cast<DWORD>(msg.length()),
                            nullptr, nullptr);
        if (!success) {
            kbase::LastError last_error;
            std::cout << "Write pipe for client " << client_pid << "failed!" << last_error;
            break;
        }
    }

    FlushFileBuffers(pipe.get());
    DisconnectNamedPipe(pipe.get());

    std::cout << "Worker for client " << client_pid << " terminates!" << std::endl;
}

void RunAsServer()
{
    std::cout << "Server is running...\n";

    while (true) {
        kbase::ScopedWinHandle pipe(
            CreateNamedPipeW(kPipeName,
                             PIPE_ACCESS_DUPLEX,
                             PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                             PIPE_UNLIMITED_INSTANCES,
                             kBufSize,
                             kBufSize,
                             0,
                             nullptr));
        ENSURE(CHECK, !!pipe).Require();

        BOOL rv = ConnectNamedPipe(pipe.get(), nullptr);
        if (rv == 0 && GetLastError() != ERROR_PIPE_CONNECTED) {
            std::cout << "[server] Failed to connect to a client!" << std::endl;
            continue;
        }

        std::thread worker(WorkerFunc, std::move(pipe));
        worker.detach();
    }
}

void RunAsClient()
{
    auto pid = GetCurrentProcessId();
    std::cout << "Client " << pid << " is running..." << std::endl;

    kbase::ScopedWinHandle pipe;

    while (true) {
        if (!WaitNamedPipeW(kPipeName, 10000)) {
            std::cout << "Client " << pid << " can't connect to server in 10s...!\n";
            return;
        }

        pipe.reset(CreateFileW(kPipeName, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING,
                               0, nullptr));
        if (pipe) {
            break;
        }

        kbase::LastError error;
        if (error.error_code() != ERROR_PIPE_BUSY) {
            std::cout << "Client " << pid << " failed to bind with the pipe; " << error;
            return;
        }
    }

    DWORD mode = PIPE_READMODE_MESSAGE;
    BOOL rv = SetNamedPipeHandleState(pipe.get(), &mode, nullptr, nullptr);
    ENSURE(CHECK, rv != 0).Require();

    std::cout << "Client " << pid << " is online!" << std::endl;

    while (true) {
        std::string msg;
        std::getline(std::cin, msg, '\n');
        if (msg == "exit") {
            return;
        }

        rv = WriteFile(pipe.get(), msg.data(), static_cast<DWORD>(msg.size()), nullptr, nullptr);
        if (!rv) {
            std::cout << "Client " << pid << " can't write to the pipe!" << std::endl;
            continue;
        }

        char buf[kBufSize] {0};
        do {
            rv = ReadFile(pipe.get(), buf, kBufSize, nullptr, nullptr);
            if (!rv && GetLastError() != ERROR_MORE_DATA) {
                std::cout << "Client " << pid << " read pipe error!\n";
                return;
            }
        } while (!rv);

        std::string echo(buf);
        std::cout << "Server echo: " << echo << std::endl;
    }
}

void TestNamedPipe()
{
    kbase::CommandLine::Init(0, nullptr);

    std::wstring process_type;
    if (kbase::CommandLine::ForCurrentProcess().GetSwitchValue(kProcessType, process_type) &&
        process_type == kProcessClient) {
        RunAsClient();
    } else {
        RunAsServer();
    }
}
