#include <cstdlib>
#include <cstring>
#include <iostream>

#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void echo(int fd)
{
    constexpr int kMaxLine = 1024;
    char buf[kMaxLine];

    size_t n = 0;
    while ((n = read(fd, buf, kMaxLine)) != 0) {
        std::string msg(buf, n);
        if (msg != "quit") {
            std::cout << "-> " << msg;
            write(fd, buf, n);
        } else {
            std::cout << "bye bye!" << std::endl;
            break;
        }
    }
}

int main(int argc, const char** argv)
{
    if (argc != 2) {
        std::cerr << "usage: <port>" << std::endl;
        return 0;
    }

    int port = atoi(argv[1]);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        std::cerr << "socket failed" << std::endl;
        return -1;
    }

    int optval = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0) {
        std::cerr << "setsockopt failed: " << errno << std::endl;
        return -1;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(static_cast<unsigned short>(port));

    if (bind(listen_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "bind failed: " << errno << std::endl;
        return -1;
    }

    if (listen(listen_fd, 10) < 0) {
        std::cerr << "listen failed: " << errno << std::endl;
        return -1;
    }

    std::cout << "server is online!" << std::endl;

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (true) {
        int conn_fd = accept(listen_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
        auto host_entry = gethostbyaddr(reinterpret_cast<const char*>(&client_addr.sin_addr.s_addr),
                                        sizeof(client_addr.sin_addr.s_addr), AF_INET);
        const char* haddrp = inet_ntoa(client_addr.sin_addr);
        std::cout << "server connected to " << host_entry->h_name << " " << haddrp << std::endl;

        echo(conn_fd);

        close(conn_fd);
    }

    return 0;
}