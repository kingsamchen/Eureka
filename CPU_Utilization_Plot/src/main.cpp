/*
 @ Kingsley Chen
*/

#include <Windows.h>

#include <conio.h>
#include <process.h>

#include <cmath>
#include <cstdint>
#include <iostream>

#include "kbase\command_line.h"
#include "kbase\memory\scoped_handle.h"
#include "kbase\sys_info.h"

const unsigned long kDefaultTimeUnit = 1000;
const wchar_t kPlotSwitchName[] = L"plot_type";
const wchar_t kSineCurve[] = L"sine";
const wchar_t kHorizon[] = L"horizon";

using PlotFunc = unsigned int (WINAPI *)(void*);

// The time unit corresponds to CPU utilization sample frequency.
// The whole utilization curve consists of curve in several time units.
void PlotTimeUnit(unsigned long time_unit, double busy_percent)
{
    unsigned long busy_span = static_cast<unsigned long>(time_unit * busy_percent);
    unsigned long time_unit_begin = GetTickCount();
    while (time_unit_begin + busy_span > GetTickCount()) {
        ;
    }

    Sleep(time_unit - busy_span);
}

unsigned int WINAPI PlotHorizontalLine(void* param)
{
    unsigned long& busy_percent_num = *static_cast<unsigned long*>(param);
    double busy_percent = static_cast<double>(busy_percent_num) / 100;

    while (true) {
        PlotTimeUnit(kDefaultTimeUnit, busy_percent);
    };

    return 0;
}

unsigned int WINAPI PlotSineCurve(void* dummy)
{
    const double kPi = 3.14;
    const unsigned long kSineCurveTimeUnit = 500;
    while (true) {
        for (double x = 0.0; x < 2 * kPi; x += 0.1) {
            double busy_percent = std::sin(x) / 2 + 0.5;
            PlotTimeUnit(kSineCurveTimeUnit, busy_percent);
        }
    }

    return 0;
}

inline void PrintUsage()
{
    std::cout << "Usage:\nCPU_Utilization_Plot.exe -plot_type=[sinecurve|horizon"
                 " {num, 20 <= num <= 80}]\n" << std::endl;
}

std::vector<kbase::ScopedSysHandle> SetupThreads(PlotFunc fn, void* param)
{
    std::vector<kbase::ScopedSysHandle> threads;
    auto core_count = kbase::SysInfo::NumberOfProcessors();
    for (size_t i = 0; i < core_count; ++i) {
        auto handle = _beginthreadex(nullptr, 0, fn, param, CREATE_SUSPENDED,
                                     nullptr);
        threads.emplace_back(reinterpret_cast<HANDLE>(handle));
        SetThreadAffinityMask(threads[i].Get(), 1U << i);
    }

    return threads;
}

int main(int argc, char* argv[])
{
    auto& commandline = kbase::CommandLine::ForCurrentProcess();

    std::wstring plot_type;
    if (!commandline.GetSwitchValue(kPlotSwitchName, &plot_type)) {
        std::cout << "Invalid instructions!" << std::endl;
        PrintUsage();
        return 1;
    }

    PlotFunc plot_fn;
    unsigned long plot_param = 0;
    if (plot_type == kHorizon) {
        auto&& arg_list = commandline.GetParameters();
        if (arg_list.empty()) {
            std::cout << "Invalid instructions!" << std::endl;
            PrintUsage();
            return 1;
        }

        plot_fn = PlotHorizontalLine;
        plot_param = std::stoul(arg_list[0]);
        if (plot_param < 20 || plot_param > 80) {
            std::cerr << "Utilization percentage number for horizontal line must "
                "range from 20 to 80" << std::endl;
            PrintUsage();
            return 1;
        }

    } else {
        plot_fn = PlotSineCurve;
    }

    std::vector<kbase::ScopedSysHandle> threads = SetupThreads(plot_fn, &plot_param);

    std::cout << "Press any key to start if you are ready" << std::endl;
    _getch();

    std::for_each(threads.begin(), threads.end(), ResumeThread);
    WaitForSingleObject(threads.front(), INFINITE);

    return 0;
}