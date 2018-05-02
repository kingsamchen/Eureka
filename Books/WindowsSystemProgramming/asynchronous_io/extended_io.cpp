/*
 @ 0xCCCCCCCC
*/

#include <fstream>
#include <iostream>
#include <vector>

#include <Windows.h>

#include "kbase/error_exception_util.h"
#include "kbase/logging.h"
#include "kbase/scoped_handle.h"
#include "kbase/path.h"
#include "kbase/file_util.h"

constexpr DWORD kReadChunkSize = 128;
constexpr DWORD kConcurrentReader = 4;

struct Params {
    LONGLONG* total;
    DWORD* in_flight;
    DWORD* read;
    bool completed;

    Params(LONGLONG* pt, DWORD* pi, DWORD* tr)
        : total(pt), in_flight(pi), read(tr), completed(false)
    {}
};

void ReadCompletion(DWORD error_code, DWORD bytes_read, LPOVERLAPPED ov)
{
    Params* p = reinterpret_cast<Params*>(ov->hEvent);
    std::cout << "[<] Read range "
              << ov->Offset << " - " << ov->Offset + bytes_read
              << "\n";

    *p->read += bytes_read;
    p->completed = true;
}

void ReadWithExtended(HANDLE file, std::vector<char>& buf)
{
    LARGE_INTEGER file_size {0};
    GetFileSizeEx(file, &file_size);

    std::cout << "[*] File size: " << file_size.QuadPart << "\n";

    LONGLONG total = file_size.QuadPart;
    buf.resize(total);

    DWORD in_flight = 0;
    DWORD data_read = 0;

    OVERLAPPED overlappeds[kConcurrentReader] {0};

    // pre-assume our file won't exceed 4-GB.

    for (int i = 0; i < kConcurrentReader && in_flight < total; ++i) {
        auto& ov = overlappeds[i];
        ov.Offset = i * kReadChunkSize;
        ov.hEvent = reinterpret_cast<HANDLE>(new Params(&total, &in_flight, &data_read));

        std::cout << "[>] Request read range " << ov.Offset << " - " << ov.Offset + kReadChunkSize
                  << "\n";

        ReadFileEx(file, &buf[ov.Offset], kReadChunkSize, &ov, ReadCompletion);

        in_flight += kReadChunkSize;
    }

    while (data_read < total) {
        DLOG(INFO) << "data read: " << data_read;
        SleepEx(0, TRUE);
        for (int i = 0; i < kConcurrentReader && in_flight < total; ++i) {
            auto& ov = overlappeds[i];
            if (static_cast<Params*>(ov.hEvent)->completed) {
                ov.Offset = in_flight;
                static_cast<Params*>(ov.hEvent)->completed = false;
                std::cout << "[>] Request read range " << ov.Offset << " - " << ov.Offset + kReadChunkSize
                          << "\n";
                ReadFileEx(file, &buf[ov.Offset], kReadChunkSize, &ov, ReadCompletion);
                in_flight += kReadChunkSize;
            }
        }
    }
}

void TestExtendedIO()
{
    kbase::Path file_path(LR"(E:\Misc\README.md)");
    kbase::ScopedWinHandle file(CreateFileW(file_path.value().c_str(),
                                            GENERIC_READ,
                                            0,
                                            nullptr,
                                            OPEN_EXISTING,
                                            FILE_FLAG_OVERLAPPED,
                                            nullptr));
    ENSURE(CHECK, !!file).Require();

    std::vector<char> file_buf;

    ReadWithExtended(file.get(), file_buf);

    file_path.AddExtension(L".bak");

    std::ofstream out(file_path.value(), std::ios_base::out | std::ios_base::binary);
    out.write(file_buf.data(), file_buf.size());

    std::cout << "Done" << std::endl;
}
