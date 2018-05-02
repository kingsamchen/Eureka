#include <setjmp.h>
#include <stdio.h>

jmp_buf g_jump_buf;

void Do(int i)
{
    if (i < 0) {
        longjmp(g_jump_buf, 1);
    }

    printf("Do\n");
}

void bar(int i)
{
    printf("bar\n");
    Do(i);
}

void foo(int i)
{
    printf("foo\n");
    bar(i);
}

int main()
{
    if (setjmp(g_jump_buf) != 0) {
        printf("error occured!\n");
        return 0;
    }

    foo(123);
    foo(-1);
    return 0;
}
