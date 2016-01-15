/*
 @ 0xCCCCCCCC
*/

#include "minidumper.h"

#include <iostream>

#include <windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "dbghelp.lib")

PEXCEPTION_POINTERS g_ex_ptr = nullptr;

BOOL ExceptionFilter(EXCEPTION_POINTERS* ex_ptr)
{
    g_ex_ptr = ex_ptr;
    return EXCEPTION_EXECUTE_HANDLER;
}

void CreateMiniDump(const std::wstring& path)
{
    __try {
        *(reinterpret_cast<int*>(0x0)) = 0xDEADBEEF;
    } __except (ExceptionFilter(GetExceptionInformation())) {
    }

    WriteMiniDumpFile(path, g_ex_ptr);
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
                                nullptr,
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
