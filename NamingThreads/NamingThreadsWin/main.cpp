/*
 @ 0xCCCCCCCC
*/

#include <cstdio>
#include <string>
#include <thread>

#include <Windows.h>
#include <processthreadsapi.h>

struct win10_description {};
struct traditional {};

void SetThreadName(win10_description)
{
    HRESULT hr = SetThreadDescription(GetCurrentThread(), L"Worker-Win10");
    if (FAILED(hr)) {
        throw std::runtime_error("SetThreadDescription failure()");
    }
}

const DWORD kVCThreadNameException = 0x406D1388;

typedef struct tagTHREADNAME_INFO {
    DWORD dwType;  // Must be 0x1000.
    LPCSTR szName;  // Pointer to name (in user addr space).
    DWORD dwThreadID;  // Thread ID (-1=caller thread).
    DWORD dwFlags;  // Reserved for future use, must be zero.
} THREADNAME_INFO;

void SetThreadName(traditional)
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = "Worker-Traditional";
    info.dwThreadID = -1;
    info.dwFlags = 0;

    __try {
        RaiseException(kVCThreadNameException, 0, sizeof(info) / sizeof(DWORD),
                       reinterpret_cast<ULONG_PTR*>(&info));
    } __except(EXCEPTION_EXECUTE_HANDLER) {
    }
}

template<typename Type>
void RunTest()
{
    std::thread th([] {
        SetThreadName(Type());
        printf("Set worker thread name\n");
        __debugbreak();
    });

    th.join();
}

int main(int argc, const char* argv[])
{
    if (argc < 2) {
        printf("exe [1|2]\n");
        return 0;
    }

    if (strcmp(argv[1], "1") == 0) {
        RunTest<win10_description>();
    } else if (strcmp(argv[1], "2") == 0) {
        RunTest<traditional>();
    }

    return 0;
}
