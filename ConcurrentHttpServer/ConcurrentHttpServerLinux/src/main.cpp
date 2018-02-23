/*
 @ 0xCCCCCCCC
*/

#include <stdio.h>

#include <thread>
#include <vector>

#include "kbase/at_exit_manager.h"
#include "kbase/command_line.h"
#include "kbase/os_info.h"

#include "tcp_connection_manager.h"
#include "worker.h"

namespace {

std::vector<std::thread> LaunchWorkers(int epfd)
{
    auto worker_count = kbase::OSInfo::GetInstance()->number_of_cores();
    std::vector<std::thread> workers;
    for (size_t i = 0; i < worker_count; ++i) {
        workers.emplace_back(std::thread(Worker(epfd)));
    }

    printf("%lu workers launched!\n", worker_count);

    return workers;
}

void QuitWorkers(std::vector<std::thread>& workers)
{
    std::for_each(workers.begin(), workers.end(), std::mem_fn(&std::thread::join));
}

}   // namespace

int main(int argc, char* argv[])
{
    kbase::CommandLine::Init(argc, argv);
    kbase::AtExitManager exit_manager;

    printf("--- KHTTPD Starts Running ---\n");

    constexpr unsigned short kPort = 8088;

    TcpConnectionManager::GetInstance()->Configure(kPort);

    auto workers = LaunchWorkers(TcpConnectionManager::GetInstance()->epfd());

    QuitWorkers(workers);

    printf("--- KHTTPD Shutdown ---\n");

    return 0;
}
