/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include <Windows.h>

#include "kbase/error_exception_util.h"
#include "kbase/file_util.h"
#include "kbase/scope_guard.h"
#include "kbase/scoped_handle.h"
#include "kbase/string_view.h"

void BasicMemoryMappedFile()
{
    kbase::ScopedWinHandle file(CreateFileW(L"test.txt",
                                         GENERIC_READ | GENERIC_WRITE,
                                         0,
                                         nullptr,
                                         OPEN_EXISTING,
                                         0,
                                         nullptr));
    ENSURE(CHECK, !!file)(kbase::LastError()).Require();

    kbase::FileInfo info;
    kbase::GetFileInfo(kbase::Path(L"test.txt"), info);

    kbase::ScopedWinHandle mapping(CreateFileMappingW(file.get(),
                                                   nullptr,
                                                   PAGE_READWRITE,
                                                   0,
                                                   1024,
                                                   L"FM"));
    ENSURE(CHECK, !!mapping)(kbase::LastError()).Require();

    auto map_view = MapViewOfFile(mapping.get(), FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    ON_SCOPE_EXIT { UnmapViewOfFile(map_view); };

    char* ptr = static_cast<char*>(map_view);
    kbase::StringView sv(ptr, 8);
    std::cout << "First 8 bytes of the file is:\n" << sv << std::endl;

    // Now we point to the first byte beyond original file content.
    ptr = static_cast<char*>(map_view) + info.file_size();
    strcpy_s(ptr, 1024 - info.file_size(), "write to mapped memory!");
}
