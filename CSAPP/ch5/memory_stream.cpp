#include <cstdio>
#include <cstring>
#include <iostream>

int main()
{
    const int kBufSize = 48;
    char buf[kBufSize];
    memset(buf, 'a', kBufSize - 2);
    buf[kBufSize-2] = '\0';
    buf[kBufSize-1] = 'X';

    FILE* fp = nullptr;
    if ((fp = fmemopen(buf, kBufSize, "w+")) == nullptr) {
        std::cerr << "fmemopen failed\n";
    }

    std::cout << "Initial buffer content: " << buf << "\n";
    fprintf(fp, "hello world");
    std::cout << "Before flush: " << buf << "\n";
    fflush(fp);
    std::cout << "After flush: " << buf << "\n";
    std::cout << "length of string in buf: " << strlen(buf) << "\n";

    memset(buf, 'b', kBufSize - 2);
    buf[kBufSize-2] = '\0';
    buf[kBufSize-1] = 'X';
    fprintf(fp, "hello world");
    fseek(fp, 0, SEEK_SET);
    std::cout << "after seek: " << buf << "\nlength of string in buf: " << strlen(buf) << "\n";

    memset(buf, 'c', kBufSize - 2);
    buf[kBufSize-2] = '\0';
    buf[kBufSize-1] = 'X';
    fprintf(fp, "hello world");
    fclose(fp);
    std::cout << "after close: " << buf << "\nlength of string in buf: " << strlen(buf);

    return 0;
}
