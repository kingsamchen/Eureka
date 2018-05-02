/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <thread>
#include <vector>

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>

#include "kbase/command_line.h"
#include "kbase/error_exception_util.h"
#include "kbase/scoped_handle.h"
#include "kbase/string_util.h"

#pragma comment(lib, "Ws2_32.lib")

const char kLocalHost[] = "127.0.0.1";
USHORT kServerPort = 12345;

const wchar_t kProcessType[] = L"type";
const wchar_t kProcessClient[] = L"client";

const char kQuitCommand[] = "$quit";

struct SocketHandleTraits {
    using Handle = SOCKET;

    SocketHandleTraits() = delete;
    ~SocketHandleTraits() = delete;

    static Handle NullHandle() noexcept
    {
        return INVALID_SOCKET;
    }

    static bool IsValid(Handle handle) noexcept
    {
        return handle != 0 && handle != INVALID_SOCKET;
    }

    static void Close(Handle handle) noexcept
    {
        closesocket(handle);
    }
};

using ScopedSocketHandle = kbase::GenericScopedHandle<SocketHandleTraits>;

struct Message {
    int32_t length;
    std::vector<char> content;

    Message()
        : length(0)
    {}

    explicit Message(const std::string& msg)
        : length(static_cast<int32_t>(msg.length())), content(msg.begin(), msg.end())
    {}
};

bool ReceiveMessage(Message& msg, SOCKET socket)
{
    int32_t remain_length = 4;

    char* header_buf = reinterpret_cast<char*>(&msg.length);
    while (remain_length > 0) {
        int bytes_read = recv(socket, header_buf, remain_length, 0);
        ENSURE(CHECK, bytes_read != SOCKET_ERROR)(WSAGetLastError()).Require();
        if (bytes_read == 0) {
            // The socket is closed.
            return false;
        }

        remain_length -= bytes_read;
        header_buf += bytes_read;
    }

    remain_length = msg.length;
    msg.content.resize(msg.length, 0);
    char* content_buf = reinterpret_cast<char*>(msg.content.data());
    while (remain_length > 0) {
        int bytes_read = recv(socket, content_buf, remain_length, 0);
        ENSURE(CHECK, bytes_read != SOCKET_ERROR)(WSAGetLastError()).Require();
        if (bytes_read == 0) {
            return false;
        }

        remain_length -= bytes_read;
        content_buf += bytes_read;
    }

    return true;
}

bool SendRequestMessage(const Message& msg, SOCKET socket)
{
    int32_t remain_length = 4;

    const char* header_buf = reinterpret_cast<const char*>(&msg.length);
    while (remain_length > 0) {
        int bytes_sent = send(socket, header_buf, remain_length, 0);
        if (bytes_sent == SOCKET_ERROR) {
            ENSURE(CHECK, kbase::NotReached())(WSAGetLastError()).Require();
            return false;
        }

        remain_length -= bytes_sent;
        header_buf += bytes_sent;
    }

    std::cout << "[*] Message header's been written: " << msg.length << "\n";

    remain_length = msg.length;
    const char* content_buf = reinterpret_cast<const char*>(msg.content.data());
    while (remain_length > 0) {
        int bytes_sent = send(socket, content_buf, remain_length, 0);
        if (bytes_sent == SOCKET_ERROR) {
            ENSURE(CHECK, kbase::NotReached())(WSAGetLastError()).Require();
            return false;
        }

        remain_length -= bytes_sent;
        content_buf += bytes_sent;
    }

    std::cout << "[*] Message content's been written: " << msg.content.size() << "\n";

    return true;
}

// Ignore potential races among outputing to stdout.
void ServerWorker(ScopedSocketHandle&& conn_socket)
{
    ScopedSocketHandle socket(std::move(conn_socket));

    while (true) {
        Message msg;

        bool success = ReceiveMessage(msg, socket.get());
        if (!success) {
            std::cout << "[*] One client exit" << std::endl;
            return;
        }

        std::string data(msg.content.begin(), msg.content.end());
        std::cout << "-> " << data << std::endl;

        kbase::ASCIIStringToUpper(data);
        msg.content.assign(data.begin(), data.end());

        SendRequestMessage(msg, socket.get());
    }
}

void RunAsSocketServer()
{
    std::cout << "--- Socket Server ---\n";

    ScopedSocketHandle listen_socket(socket(AF_INET, SOCK_STREAM, 0));

    sockaddr_in srv_addr {0};
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(kServerPort);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    constexpr int kMaxClients = 10;
    if (bind(listen_socket.get(), reinterpret_cast<sockaddr*>(&srv_addr), sizeof(srv_addr)) ||
        listen(listen_socket.get(), kMaxClients)) {
        ENSURE(CHECK, kbase::NotReached())(WSAGetLastError()).Require();
        return;
    }

    sockaddr_in client_addr {0};
    int client_addr_len = sizeof(client_addr);
    while (true) {
        ScopedSocketHandle conn_socket(accept(listen_socket.get(),
                                              reinterpret_cast<sockaddr*>(&client_addr),
                                              &client_addr_len));
        ENSURE(CHECK, !!conn_socket).Require();

        std::cout << "[*] Connected with client " << client_addr.sin_port << "\n";

        std::thread worker(ServerWorker, std::move(conn_socket));
        worker.detach();
    }
}

void RunAsSocketClient()
{
    std::cout << "--- Socket Client ---\n";

    ScopedSocketHandle client_socket(socket(AF_INET, SOCK_STREAM, 0));

    sockaddr_in client_addr {0};
    client_addr.sin_family = AF_INET;
    inet_pton(AF_INET, kLocalHost, &client_addr.sin_addr.s_addr);
    client_addr.sin_port = htons(kServerPort);

    auto rv = connect(client_socket.get(), reinterpret_cast<sockaddr*>(&client_addr),
                      sizeof(client_addr));
    ENSURE(CHECK, rv == 0)(WSAGetLastError()).Require();

    std::cout << "[*] Connected with the server!\n" << std::endl;

    while (true) {
        std::string data;
        std::getline(std::cin, data);
        if (data == kQuitCommand) {
            std::cout << "Bye!";
            shutdown(client_socket.get(), SD_BOTH);
            return;
        }

        Message msg(data);
        SendRequestMessage(msg, client_socket.get());

        Message response;
        if (!ReceiveMessage(response, client_socket.get())) {
            std::cerr << "Receive response error!" << std::endl;
        }

        ENSURE(CHECK, response.length == response.content.size()).Require();
        std::cout << "-> ";
        std::cout.write(response.content.data(), response.content.size());
        std::cout << std::endl;
    }
}

void TestSocketMessage()
{
    kbase::CommandLine::Init(0, nullptr);

    WSAData wsa_data;
    WSAStartup(MAKEWORD(2, 0), &wsa_data);

    std::wstring process_type;
    if (kbase::CommandLine::ForCurrentProcess().GetSwitchValue(kProcessType, process_type) &&
        process_type == kProcessClient) {
        RunAsSocketClient();
    } else {
        RunAsSocketServer();
    }

    WSACleanup();
}
