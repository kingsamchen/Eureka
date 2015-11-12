/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <functional>

#include "signals.h"

void foo(int i, const int& j, int&& k)
{
    std::cout << "foo" << std::endl;
}

struct Foo {
    static void sfoo(int i, const int& j, int&& k)
    {
        std::cout << "Foo::sfoo" << std::endl;
    }

    void foo(int i, const int& j, int&& k)
    {
        s = "world";
        std::cout << "Foo::foo" << std::endl;
    }

    std::string s = "hello";
};

int main()
{
    Signal<int, const int&, int&&> s;
    Slot slot = s.Connect(&foo);
    s.Connect(&Foo::sfoo);
    auto foo = std::make_shared<Foo>();
    auto pf = foo.get();
    using namespace std::placeholders;
    s.Connect(std::bind(&Foo::foo, pf, _1, _2, _3), foo);
    s.Emit(1, 3, 5);
    slot.Disconnect();
    s.Emit(5, 3, 1);
    foo.reset();
    s.Emit(5, 3, 1);
    s.DisconnectAll();
    s.Emit(3, 2, 1);
    return 0;
}