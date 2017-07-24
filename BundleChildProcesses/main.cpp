#include <conio.h>

#include <cassert>
#include <iostream>
#include <string>

#include <Windows.h>

bool LaunchAndWait(const std::wstring& cmdline, HANDLE job)
{
    wchar_t buf[255] {0};
    wcscpy_s(buf, cmdline.c_str());

    STARTUPINFO startup {0};
    startup.cb = sizeof(startup);
    PROCESS_INFORMATION process_information {nullptr};

    if (!CreateProcessW(nullptr,
                        buf,
                        nullptr,
                        nullptr,
                        FALSE,
                        CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB,
                        nullptr,
                        nullptr,
                        &startup,
                        &process_information)) {
        auto err = GetLastError();
        std::cerr << "Failed to create process: " << err;
        return false;
    }

    if (!AssignProcessToJobObject(job, process_information.hProcess)) {
        std::cerr << "Failed to assign process to job: " << GetLastError();
        return false;
    }

    ResumeThread(process_information.hThread);

    CloseHandle(process_information.hProcess);
    CloseHandle(process_information.hThread);

    return true;
}

int main()
{
    BOOL is_in_job;
    IsProcessInJob(GetCurrentProcess(), nullptr, &is_in_job);
    std::cout << "Is the main process in job: " << is_in_job << std::endl;

    std::cout << "Wait for constructing job object!\n";

    HANDLE job = CreateJobObjectW(nullptr, nullptr);
    if (!job) {
        auto err = GetLastError();
        std::cerr << "Failed to create job: " << err << std::endl;
        return 0;
    }

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION job_limits;
    memset(&job_limits, 0, sizeof(job_limits));
    job_limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

    if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation, &job_limits, sizeof(job_limits))) {
        auto err = GetLastError();
        std::cerr << "Failed to set job information: " << err << std::endl;
    }

    std::cout << "Job object created! Launch test subjects...\n";

    bool r = LaunchAndWait(LR"(C:\Windows\notepad.exe)", job);
    assert(r);

    _getch();

    return 0;
}