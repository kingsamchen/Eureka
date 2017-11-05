/*
 @ 0xCCCCCCCC
*/

#include <array>
#include <cstring>
#include <iostream>

#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const char kPipeName[] = "global_pipe";
const char kClientProcess[] = "--type=client";

void RunAsClient()
{
    int fifo_fd = open(kPipeName, O_RDWR);
    if (fifo_fd == -1) {
        std::cerr << "Open fifo failed\n";
        return;
    }

    std::cout << "Client is online!\n";

    std::string msg;
    std::array<char, PIPE_BUF> buf;
    while (true) {
        std::getline(std::cin, msg);
        write(fifo_fd, msg.data(), msg.size());

        ssize_t bytes_read = read(fifo_fd, buf.data(), buf.size());
        std::cout << "=> ";
        std::cout.write(buf.data(), bytes_read);
        std::cout << std::endl;
    }
}

void RunAsServer()
{
    if (mkfifo(kPipeName, 0777) != 0) {
        std::cerr << "Create fifo at " << kPipeName << " failed\n";
        return;
    }

    int fifo_fd = open(kPipeName, O_RDWR);
    if (fifo_fd == -1) {
        std::cerr << "Open fifo failed\n";
        return;
    }

    std::cout << "Server is online!\n";

    std::array<char, PIPE_BUF> buf;
    while (true) {
        auto bytes_read = read(fifo_fd, buf.data(), buf.size());
        if (strncmp(buf.data(), "$quit", 5) == 0) {
            std::cout << "Bye!" << std::endl;
        }

        std::cout << "-> ";
        std::cout.write(buf.data(), bytes_read);
        std::cout << std::endl;

        for (ssize_t i = 0; i < bytes_read; ++i) {
            buf[i] = toupper(buf[i]);
        }

        write(fifo_fd, buf.data(), bytes_read);
    }
}

int main(int argc, char* argv[])
{
    bool client_mode = false;
    if (argc > 1) {
        client_mode = strcmp(kClientProcess, argv[1]) == 0;
    }

    if (client_mode) {
        RunAsClient();
    } else {
        RunAsServer();
    }

    return 0;
}
