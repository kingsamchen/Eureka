/*
 @ 0xCCCCCCCC
*/

#include "failure_case.h"
#include "with_sfinae.h"

void TestRaw()
{
    using namespace raw;

    int i = 0;
    Bar bar;

    Foo(i);

    // Ambiguous call! because the first Foo is still considered even if the instantiation code will
    // be illegal.
    //Foo(bar);
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

    // ProcessIntOnly(3.14);
}

int main()
{
    TestProcessIntOnly();
    return 0;
}
