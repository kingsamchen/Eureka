/*
 @ 0xCCCCCCCC
*/

#include "tcp_connection_manager.h"

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "kbase/error_exception_util.h"

#include "tcp_connection.h"

namespace {

void MakeFDNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int rv = fcntl(fd, F_SETFL, flags);
    ENSURE(CHECK, rv != -1).Require();
}

void AddFDToPollCandidates(int epfd, int fd, void* data)
{
    epoll_event ev {0};
    ev.events = EPOLLIN | EPOLLET | EPOLLOUT;
    ev.data.ptr = data;
    auto rv = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    ENSURE(CHECK, rv != -1)(errno).Require();
}

ScopedFD CreateListener(unsigned short port, int max_pending_clients)
{
    ScopedFD listener(socket(AF_INET, SOCK_STREAM, 0));

    sockaddr_in server_addr {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listener.get(), reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
    listen(listener.get(), max_pending_clients);

    MakeFDNonBlocking(listener.get());

    printf("nonblocking listener is created and is listening on %d\n", port);

    return listener;
}

}   // namespace

// static
TcpConnectionManager* TcpConnectionManager::GetInstance()
{
    return kbase::Singleton<TcpConnectionManager>::instance();
}

TcpConnectionManager::TcpConnectionManager()
    : configured_(false)
{}

void TcpConnectionManager::Configure(unsigned short port)
{
    epfd_.reset(epoll_create1(0));
    printf("epoll fd has been created!\n");

    listener_ = CreateListener(port, kMaxPendingClients);

    AddFDToPollCandidates(epfd_.get(), listener_.get(), this);

    configured_ = true;
}

void TcpConnectionManager::AcceptNewClient()
{
    while (true) {
        struct sockaddr_in addr {0};
        socklen_t addr_len = sizeof(addr);
        int client_fd = accept(listen_fd(), reinterpret_cast<sockaddr*>(&addr), &addr_len);
        if (client_fd == -1) {
            if (errno == EAGAIN) {
                printf("All incoming connection requests are processed\n");
            } else {
                printf("accept() failed!\n");
            }

            return;
        } else {
            MakeFDNonBlocking(client_fd);

            auto conn = std::make_unique<TcpConnection>(ScopedFD(client_fd), addr);

            AddFDToPollCandidates(epfd(), client_fd, conn.get());

            // To simplify our connection management logic, we assume that we don't reuse connection
            // instances.

            {
                std::lock_guard<std::mutex> lock(conn_mutex_);
                connections_.push_back(std::move(conn));
            }
        }
    }
}