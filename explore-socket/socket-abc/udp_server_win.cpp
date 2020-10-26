#include <array>

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    WSADATA wsaData;
    auto iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        printf("Error: WSAStartup()\n");
        return 1;
    }

    auto srv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (srv_sock == INVALID_SOCKET) {
        printf("Error: invalid server udp socket");
        return 1;
    }

    sockaddr_in srv_addr {};
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = INADDR_ANY;
    srv_addr.sin_port = htons(9876);

    auto rv = bind(srv_sock, reinterpret_cast<sockaddr*>(&srv_addr), sizeof(srv_addr));
    if (rv != 0) {
        printf("Error: bind failure; %d", WSAGetLastError());
        return 1;
    }

    sockaddr_in client_addr {};
    int client_len = sizeof(client_addr);
    std::array<char, 1024> buf;
    while (true) {
        auto n = recvfrom(srv_sock,
                          buf.data(),
                          buf.size(),
                          0,
                          reinterpret_cast<sockaddr*>(&client_addr),
                          &client_len);
        if (n < 0) {
            printf("Error: recvform failure; %d\n", WSAGetLastError());
            break;
        }

        buf[n] = '\0';

        printf("->: %s\n", buf.data());

        sendto(srv_sock, "ACK", 3, 0, reinterpret_cast<sockaddr*>(&client_addr), client_len);
    }


    closesocket(srv_sock);

    WSACleanup();

    return 0;
}
