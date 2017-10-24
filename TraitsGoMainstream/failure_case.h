/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <iostream>
#include <string>

namespace raw {

inline void Foo(int param)
{
    std::cout << "-- void Foo(int) --\n" << param << std::endl;
}

template<typename T>
typename T::result_type Foo(const T& param)
{
    std::cout << "-- typename T::result_type Foo(const T&) --\n" << param << std::endl;
    return typename T::result_type();
}

struct Bar {
    using result_type = bool;
};

inline std::ostream& operator<<(std::ostream& out, const Bar&)
{
    out << "struct Bar\n";
    return out;
}

}   // namespace raw
