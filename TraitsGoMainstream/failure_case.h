/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <iostream>
#include <string>

namespace raw {

inline unsigned long long Foo(unsigned long long param)
{
    std::cout << "-- void Foo(unsigned long long) --\n" << param << std::endl;
    return param;
}

template<typename T>
typename T::result_type Foo(const T& param)
{
    std::cout << "-- typename T::result_type Foo(const T&) --\n" << param << std::endl;
    return param.print();
}

struct Bar {
    using result_type = bool;
    result_type print() const
    {
        std::cout << "hello from bar!" << std::endl;
        return true;
    }
};

inline std::ostream& operator<<(std::ostream& out, const Bar&)
{
    out << "struct Bar\n";
    return out;
}

}   // namespace raw
