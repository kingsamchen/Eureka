#include <cstring>
#include <iostream>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, const char** argv)
{
    if (argc != 3) {
        std::cerr << "usage: <host> <port>" << std::endl;
        return 0;
    }

    const char* host = argv[1];
    int port = atoi(argv[2]);

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        std::cerr << "socket failed" << std::endl;
        return -1;
    }

    auto host_entry = gethostbyname(host);
    if (!host_entry) {
        std::cerr << "gethostbyname failed for: " << host << "; " << errno << std::endl;
        return -1;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, host_entry->h_addr_list[0], host_entry->h_length);
    server_addr.sin_port = htons(port);

    if (connect(client_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "connect failed: " << errno << std::endl;
        return -1;
    }

    std::cout << "client is online!" << std::endl;

    constexpr int kMaxLine = 1024;
    char buf[1024];

    while (fgets(buf, kMaxLine, stdin) != nullptr) {
        write(client_fd, buf, strlen(buf));

        if (strcmp(buf, "quit") == 0) {
            break;
        }

        read(client_fd, buf, kMaxLine);
        fputs("-> ", stdout);
        fputs(buf, stdout);
    }

    close(client_fd);

    return 0;
}
