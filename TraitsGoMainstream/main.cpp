/*
 @ 0xCCCCCCCC
*/

#include <conio.h>

#include "failure_case.h"
#include "with_sfinae.h"

void TestRaw()
{
    using namespace raw;

    Bar bar;

    std::cout << "Foo(10) = " << Foo(10) << std::endl;

    std::cout << "Foo(bar) = " << Foo(bar) << std::endl;
}

void TestSFINAE()
{
    using namespace sf;

    int i = 0;
    std::string s = "hello";
    Bar bar;

    Foo(i);
    Foo(s);
    Foo(bar);
}

void TestProcessIntOnly()
{
    using namespace sf;
    int i = 1;
    long j = 2L;
    ProcessIntOnly(i);
    ProcessIntOnly(j);

    //ProcessIntOnly(3.14);
}

int main()
{
    TestRaw();
    TestSFINAE();
    TestProcessIntOnly();
    _getch();
    return 0;
}
