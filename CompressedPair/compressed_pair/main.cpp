/*
 @ 0xCCCCCCCC
*/

#include <algorithm>
#include <iostream>

#include "compressed_pair.h"

template<typename T>
struct default_printer {
    void operator()(const T& ele)
    {
        std::cout << ele << std::endl;
    }
};

template<typename T, typename P = default_printer<T>>
class OutputChannel {
public:
    template<typename U>
    explicit OutputChannel(U&& data)
        : cp_(std::forward<U>(data))
    {}

    template<typename U, typename V>
    OutputChannel(U&& data, V&& p)
        : cp_(std::forward<V>(p), std::forward<U>(data))
    {}

    void Log()
    {
        cp_.get_first()(cp_.second);
    }

private:
    CompressedPair<P, T> cp_;
};

void SPrint(const std::string& s)
{
    std::cout << "[" << s << "]" << std::endl;
}

int main()
{
    std::cout << "sizeof(std::string) = " << sizeof(std::string) << std::endl;

    OutputChannel<std::string> lc("hello world");
    std::cout << "sizeof(lc) = " << sizeof(lc) << std::endl;
    static_assert(sizeof(std::string) == sizeof(lc), "error");
    lc.Log();

    OutputChannel<std::string, void(*)(const std::string&)> lsc("hello world", &SPrint);
    std::cout << "sizeof(lsc) = " << sizeof(lsc) << std::endl;
    static_assert(sizeof(std::string) == sizeof(lsc) - sizeof(void*), "error");
    lsc.Log();

    auto up_printer = [](const std::string& s) {
        std::string us;
        std::transform(s.begin(), s.end(), std::back_inserter(us),
                       [](char c) { return static_cast<char>(toupper(c)); });
        std::cout << us << std::endl;
    };

    OutputChannel<std::string, decltype(up_printer)> usc("hello world", up_printer);
    std::cout << "sizeof(usc) = " << sizeof(usc) << std::endl;
    static_assert(sizeof(std::string) == sizeof(usc), "error");
    usc.Log();

    return 0;
}
