
#include <stdio.h>

void show_bytes(const void* data, size_t size)
{
    const unsigned char* block = (const unsigned char*)data;
    for (size_t i = 0; i < size; ++i) {
        printf("%.2x ", *block++);
    }

    printf("\n");
}


int main()
{
    int x = 53191;
    short sx = (short)x;
    show_bytes(&x, sizeof(x));
    show_bytes(&sx, sizeof(sx));
    return 0;
}
