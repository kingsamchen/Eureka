/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <fstream>
#include <vector>

#include <Windows.h>

#include "kbase/error_exception_util.h"
#include "kbase/scoped_handle.h"
#include "kbase/path.h"
#include <kbase/file_util.h>

constexpr DWORD kReadChunkSize = 1024;
constexpr DWORD kConcurrentReader = 4;

void ClearOverlapped(OVERLAPPED& ov)
{
    ResetEvent(ov.hEvent);
    memset(&ov, 0, sizeof(ov));
}

std::vector<kbase::ScopedWinHandle> BuildEvents(DWORD count)
{
    std::vector<kbase::ScopedWinHandle> events;

    for (DWORD i = 0; i < count; ++i) {
        events.emplace_back(CreateEventW(nullptr, TRUE, FALSE, L""));
    }

    return events;
}

void ReadWithOverlapped(HANDLE file, std::vector<char>& buf)
{
    OVERLAPPED overlappeds[kConcurrentReader] {0};

    LARGE_INTEGER file_size {0};
    GetFileSizeEx(file, &file_size);

    std::cout << "[*] File size: " << file_size.QuadPart << "\n";

    auto total = file_size.QuadPart;
    DWORD request_bytes = 0;
    DWORD total_read = 0;

    buf.resize(total);

    std::vector<kbase::ScopedWinHandle> events = BuildEvents(kConcurrentReader);
    HANDLE raw_events[kConcurrentReader];
    for (int i = 0; i < kConcurrentReader; ++i) {
        raw_events[i] = events[i].get();
    }

    // Initiate
    for (int i = 0; i < kConcurrentReader; ++i) {
        auto& ov = overlappeds[i];

        ov.hEvent = raw_events[i];

        LARGE_INTEGER offset {0};
        offset.QuadPart = request_bytes;

        ov.Offset = offset.LowPart;
        ov.OffsetHigh = offset.HighPart;

        std::cout << "[>] Request read range " << request_bytes << " - " << request_bytes + kReadChunkSize << "\n";

        ReadFile(file, &buf[offset.QuadPart], kReadChunkSize, nullptr, &ov);

        request_bytes += kReadChunkSize;
        if (request_bytes >= total) {
            break;
        }
    }

    while (true) {
        DWORD done_reader =
            WaitForMultipleObjects(kConcurrentReader, raw_events, FALSE, INFINITE) - WAIT_OBJECT_0;
        auto& ov = overlappeds[done_reader];

        DWORD bytes_read = 0;
        GetOverlappedResult(file, &ov, &bytes_read, FALSE);

        std::cout << "[<] Read range " << ov.Offset << " - " << bytes_read + ov.Offset << "\n";
        total_read += bytes_read;

        if (total_read >= total) {
            break;
        }

        ClearOverlapped(ov);

        if (request_bytes < total) {
            ov.hEvent = raw_events[done_reader];
            LARGE_INTEGER offset {0};
            offset.QuadPart = request_bytes;
            ov.Offset = offset.LowPart;
            ov.OffsetHigh = offset.HighPart;

            std::cout << "[>] Request read range " << request_bytes << " - " << request_bytes + kReadChunkSize << "\n";

            ReadFile(file, &buf[request_bytes], kReadChunkSize, nullptr, &ov);

            request_bytes += kReadChunkSize;
        }
    }
}

void TestOverlappedIO()
{
    kbase::Path file_path(LR"(C:\Programs\bililive\livehime\livehime.exe)");
    kbase::ScopedWinHandle file(CreateFileW(file_path.value().c_str(),
                                            GENERIC_READ,
                                            0,
                                            nullptr,
                                            OPEN_EXISTING,
                                            FILE_FLAG_OVERLAPPED,
                                            nullptr));
    ENSURE(CHECK, !!file).Require();

    std::vector<char> file_buf;

    ReadWithOverlapped(file.get(), file_buf);

    file_path.AddExtension(L".bak");

    std::ofstream out(file_path.value(), std::ios_base::out | std::ios_base::binary);
    out.write(file_buf.data(), file_buf.size());

    std::cout << "Done" << std::endl;
}