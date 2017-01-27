/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <string>

#define DECLARE_HAS_CLASS_MEMBER(NAME)                                                  \
template<typename T, typename... Args>                                                  \
struct has_member_##NAME {                                                              \
    template<typename U>                                                                \
    constexpr static auto check(const void*)->                                          \
        decltype(std::declval<U>().NAME(std::declval<Args>()...), std::true_type());    \
                                                                                        \
    template<typename U>                                                                \
    constexpr static std::false_type check(...);                                        \
                                                                                        \
    static constexpr bool value = decltype(check<T>(nullptr))::value;                   \
}

#define HAS_CLASS_MEMBER(CLASS, MEMBER, ...) \
    has_member_##MEMBER<CLASS, __VA_ARGS__>::value

DECLARE_HAS_CLASS_MEMBER(foo);
DECLARE_HAS_CLASS_MEMBER(bar);

struct Foo {
    void foo() {}
};

struct Bar {
    void bar() { std::cout << "Bar::bar()\n"; }
    void foo(const std::string&, int) { std::cout << "Bar::foo(const std::string&, int)\n"; }
};

template<typename T>
typename std::enable_if<HAS_CLASS_MEMBER(T, foo, const std::string&, int)>::type RunFoo(T& obj)
{
    obj.foo("hello world", 1);
}

template<typename T>
typename std::enable_if<!HAS_CLASS_MEMBER(T, foo, const std::string&, int)>::type RunFoo(T& obj)
{
    obj.bar();
}

int main()
{
    Bar bar;
    RunFoo(bar);
    return 0;
}