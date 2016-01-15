/*
 @ 0xCCCCCCCC
*/

#include "minidumper.h"

#include <iostream>

#include <windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "dbghelp.lib")

void ExceptionFilter(const std::wstring& path, EXCEPTION_POINTERS* ex_ptr)
{
    WriteMiniDumpFile(path, ex_ptr);
}

void CreateMiniDump(const std::wstring& path)
{
    __try {
        RaiseException(EXCEPTION_BREAKPOINT, 0, 0, nullptr);
    } __except (ExceptionFilter(path, GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {
    }
}

void WriteMiniDumpFile(const std::wstring& path, EXCEPTION_POINTERS* exception_pointers)
{
    HANDLE file = CreateFileW(path.c_str(),
                              GENERIC_WRITE,
                              0,
                              nullptr,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        DWORD error_code = GetLastError();
        std::cerr << "create dump file failed: " << error_code << std::endl;
        return;
    }

    MINIDUMP_EXCEPTION_INFORMATION exception_information { 0 };
    exception_information.ThreadId = GetCurrentThreadId();
    exception_information.ClientPointers = FALSE;
    exception_information.ExceptionPointers = exception_pointers;

    BOOL rv = MiniDumpWriteDump(GetCurrentProcess(),
                                GetCurrentProcessId(),
                                file,
                                MiniDumpNormal,
                                &exception_information,
                                nullptr,
                                nullptr);
    if (!rv) {
        DWORD error_code = GetLastError();
        std::cerr << "write dump file failed: " << error_code << std::endl;
        CloseHandle(file);
        return;
    }

    CloseHandle(file);

    std::cout << "dump file created\n";
}
