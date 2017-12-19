/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include <Windows.h>

void CALLBACK IOCompletionCallback(PTP_CALLBACK_INSTANCE instance, PVOID context, PVOID overlapped,
                                   ULONG io_result, ULONG_PTR bytes_transferred, PTP_IO io)
{
    char* data = static_cast<char*>(context);
    auto ov = static_cast<OVERLAPPED*>(overlapped);

    std::cout.write(data, ov->InternalHigh);
}

void TestThreadpoolIO()
{
    HANDLE file = CreateFileW(L"LICENSE", GENERIC_READ, 0, nullptr, OPEN_EXISTING,
                              FILE_FLAG_OVERLAPPED, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
        std::cerr << "Cannot open the LICENSE file!" << std::endl;
        return;
    }

    constexpr DWORD kChunkSize = 4096;

    char buf[kChunkSize];

    auto aio_id = CreateThreadpoolIo(file, IOCompletionCallback, buf, nullptr);
    if (!aio_id) {
        std::cerr << "Failed to create IO thread pool" << std::endl;
        return;
    }

    OVERLAPPED ov {0};
    DWORD bytes_read = 0;
    while (true) {
        // Cripple...
        if (ov.InternalHigh == 0 && ov.Offset != 0) {
            break;
        }

        memset(&ov, 0, sizeof(ov));
        ov.Offset = bytes_read;
        StartThreadpoolIo(aio_id);
        if (!ReadFile(file, buf, kChunkSize, nullptr, &ov)) {
            if (GetLastError() != ERROR_IO_PENDING) {
                CancelThreadpoolIo(aio_id);
                std::cout << "\nAll DONE" << std::endl;
                break;
            }
        }

        bytes_read += kChunkSize;

        WaitForThreadpoolIoCallbacks(aio_id, FALSE);
    }
}
