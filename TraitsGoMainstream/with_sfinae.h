/*
@ 0xCCCCCCCC
*/

#pragma once

#include <iostream>
#include <string>
#include <type_traits>

namespace sf {

struct Bar {
    using result_type = bool;
};

inline std::ostream& operator<<(std::ostream& out, const Bar&)
{
    out << "struct Bar\n";
    return out;
}

// Illegal constructs trigger SFINAE, and the entire function template is ignored.

template<typename T>
std::enable_if_t<!std::is_class_v<T>> Foo(const T& param)
{
    std::cout << "-- Foo for non-class --\n" << param << std::endl;
}

template<typename T>
std::enable_if_t<std::is_class_v<T>> Foo(const T& param)
{
    std::cout << "-- Foo for class --\n" << param << std::endl;
}

template<typename T>
void ProcessIntOnly(T n, std::enable_if_t<std::is_integral_v<T>, T>* t = nullptr)
{
    std::cout << "integral -> " << n << std::endl;
}

}   // namespace sf