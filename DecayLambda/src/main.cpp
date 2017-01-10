#include <iostream>
#include <string>

template<typename T>
struct dememberize;

template<typename C, typename R, typename... Args>
struct dememberize<R(C::*)(Args...) const> {
    using type = R(*)(Args...);
};

template<typename T>
struct lambda_pointerize_impl {
    using type = typename dememberize<decltype(&T::operator())>::type;
};

template<typename T>
using lambda_pointerize = typename lambda_pointerize_impl<T>::type;

template<typename F>
lambda_pointerize<F> lambda_decay(F lambda)
{
    return lambda_pointerize<F>(lambda);
}

using lpfn = void(*)(const std::string&);

lpfn Foo()
{
    return lambda_decay([](const std::string& s) { std::cout << s << std::endl; });
}

int main()
{
    auto ptr = Foo();
    ptr("hello world");
    return 0;
}