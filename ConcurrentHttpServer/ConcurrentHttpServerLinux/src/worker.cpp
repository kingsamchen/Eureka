/*
 @ 0xCCCCCCCC
*/

#include "worker.h"

#include <stdio.h>

#include <vector>

#include <unistd.h>
#include <sys/epoll.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "tcp_connection.h"
#include "tcp_connection_manager.h"

Worker::Worker(int epfd)
    : epfd_(epfd)
{}

void Worker::operator()() const
{
    printf("Worker %ld starts to run\n", syscall(__NR_gettid));
    WorkProc();
}

void Worker::WorkProc() const
{
    constexpr int kMaxEvents = 64;
    std::vector<epoll_event> active_events(kMaxEvents);

    while (true) {
        int active_count = epoll_wait(epfd_, active_events.data(), kMaxEvents, -1);
        for (int i = 0; i < active_count; ++i) {
            auto& ev = active_events[i];
            if ((ev.events & EPOLLERR) || (ev.events & EPOLLHUP)) {
                printf("epoll error!\n");
                // TODO: handle error
            } else if (ev.data.ptr == TcpConnectionManager::GetInstance()) {
                TcpConnectionManager::GetInstance()->AcceptNewClient();
            } else {
                auto conn = static_cast<TcpConnection*>(ev.data.ptr);
                if (ev.events & EPOLLIN) {
                    conn->Read();
                } else if (ev.events & EPOLLOUT) {
                    conn->Write();
                } else {
                    printf("Unexpected epoll events: %x\n", ev.events);
                }
            }
        }
    }
}