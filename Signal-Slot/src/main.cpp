/*
 @ 0xCCCCCCCC
*/

#include <iostream>

#include "signals.h"

void foo(int i, const int& j, int&& k)
{
    std::cout << "foo" << std::endl;
}

struct Foo {
    static void foo(int i, const int& j, int&& k)
    {
        std::cout << "Foo::foo" << std::endl;
    }
};

int main()
{
    Signal<int, const int&, int&&> s;
    std::function<void(int, const int&, int&&)> f(&foo);
    Slot slot = s.Connect(&foo);
    s.Connect(&Foo::foo);
    s.Connect(f);
    s.Emit(1, 3, 5);

    slot.Disconnect();
    s.Emit(5, 3, 1);

    return 0;
}