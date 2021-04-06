/*
 @ 0xCCCCCCCC
*/

#include <stdio.h>
#include <string.h>

#include <iostream>
#include <vector>

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void SetFDNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl() getfl");
        return;
    }

    int rv = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if (rv == -1) {
        perror("fcntl() setfl");
    }
}

bool HandleAccept(int ep_fd, int listen_fd)
{
    while (true) {
        struct sockaddr in_addr;
        socklen_t in_addr_len = sizeof(in_addr);
        int client_fd = accept(listen_fd, &in_addr, &in_addr_len);
        if (client_fd == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cout << "All connection requests are processed!\n";
                break;
            } else {
                std::cerr << "accpet() failure\n";
                return false;
            }
        } else {
            std::cout << "Accepting a new connection\n";

            SetFDNonBlocking(client_fd);

            struct epoll_event event;
            memset(&event, 0, sizeof(event));
            event.data.fd = client_fd;
            event.events = EPOLLIN | EPOLLET;

            if (epoll_ctl(ep_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                std::cerr << "Failed to add client fd into epoll list\n";
                return false;
            }
        }
    }

    return true;
}

void HandleRead(int client_fd)
{
    const int kBufSize = 255;
    char buf[kBufSize];

    while (true) {
        auto bytes_read = read(client_fd, buf, kBufSize);
        if (bytes_read == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cout << "No more data on client fd " << client_fd << std::endl;
                break;
            } else {
                std::cerr << "read on client fd failure: " << client_fd << std::endl;
                _exit(1);
            }
        } else if (bytes_read == 0) {
            std::cout << "finished with client fd: " << client_fd << std::endl;
            close(client_fd);
        } else {
            std::cout << "-> ";
            std::cout.write(buf, bytes_read);
        }
    }
}

int main()
{
    const short kPort = 12345;

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket()");
        return 1;
    }

    int enable = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        perror("setsockopt()");
        return 1;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(kPort);

    if (bind(listen_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
        perror("bind()");
        return 1;
    }

    SetFDNonBlocking(listen_fd);

    if (listen(listen_fd, SOMAXCONN) < 0) {
        perror("listen()");
        return 1;
    }

    int ep_fd = epoll_create1(0);
    if (ep_fd == -1) {
        perror("epoll_create1()");
        return 1;
    }

    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.data.fd = listen_fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(ep_fd, EPOLL_CTL_ADD, listen_fd, &event) == -1) {
        perror("epoll_ctl() ADD");
        return 1;
    }

    const int kMaxEvents = 64;
    std::vector<struct epoll_event> incoming_events(kMaxEvents);
    while (true) {
        std::cout << "Waiting...\n";
        int event_cnt = epoll_wait(ep_fd, incoming_events.data(), kMaxEvents, -1);
        if (event_cnt == -1) {
            perror("epoll_wait()");
            return 1;
        }

        for (int i = 0; i < event_cnt; ++i) {
            if ((incoming_events[i].events & EPOLLERR) || (incoming_events[i].events & EPOLLHUP) ||
                !(incoming_events[i].events & EPOLLIN)) {
                std::cerr << "epoll error at fd: " << incoming_events[i].data.fd << std::endl;
                close(incoming_events[i].data.fd);
            } else if (incoming_events[i].data.fd == listen_fd) {
                if (!HandleAccept(ep_fd, listen_fd)) {
                    return 1;
                }
            } else {
                HandleRead(incoming_events[i].data.fd);
            }
        }
    }

    return 0;
}
