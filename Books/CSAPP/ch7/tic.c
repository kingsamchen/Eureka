#include <stdio.h>

void f();

int x = 12345;
int y = 54321;

int main()
{
    f();
    printf("x = 0x%x y = 0x%x \n", x, y);
    return 0;
}

