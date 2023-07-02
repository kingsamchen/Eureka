//
// Kingsley Chen
//

#include <cstdint>
#include <tuple>
#include <variant>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

//
// Definition of typelist
//

template<typename... Ts>
struct typelist {
    struct is_typelist {};

    using type = typelist; // type is self

    static constexpr std::size_t size = sizeof...(Ts); // length of the list

    template<typename... Us>
    using prepend = typelist<Us..., Ts...>;

    template<typename... Us>
    using append = typelist<Ts..., Us...>;

    template<template<typename...> typename T>
    using to = T<Ts...>;
};

template<typename T>
concept is_typelist =
        requires {
            typename T::is_typelist;
            typename T::type;
        };

TEST_CASE("basics") {
    using list = typelist<int, char>;
    static_assert(is_typelist<list>);
    static_assert(list::size == 2);
    static_assert(std::is_same_v<list::prepend<double>, typelist<double, int, char>>);
    static_assert(std::is_same_v<list::append<double>, typelist<int, char, double>>);
    static_assert(std::is_same_v<list::to<std::tuple>, std::tuple<int, char>>);
    static_assert(std::is_same_v<list::to<std::variant>, std::variant<int, char>>);
}

//
// map
//

// is_typelist can be replace with normal typename
template<is_typelist In, template<typename> typename F>
struct map;

template<template<typename> typename F, typename... Ts>
struct map<typelist<Ts...>, F> : typelist<typename F<Ts>::type...> {};

template<is_typelist In, template<typename> typename F>
using map_t = typename map<In, F>::type;

TEST_CASE("map") {
    using list = typelist<int, char, float, double>;
    static_assert(std::is_same_v<map_t<list, std::add_pointer>,
                                 typelist<int*, char*, float*, double*>>);
}

//
// filter
//

template<is_typelist In, template<typename> typename P, is_typelist Out = typelist<>>
struct filter : Out {};

template<template<typename> typename P, is_typelist Out, typename H, typename... Ts>
struct filter<typelist<H, Ts...>, P, Out>
    : std::conditional_t<P<H>::value,
                         filter<typelist<Ts...>, P, typename Out::template append<H>>,
                         filter<typelist<Ts...>, P, Out>> {};

template<is_typelist In, template<typename> typename P>
using filter_t = typename filter<In, P>::type;

template<typename T>
struct check_size {
    static constexpr bool value = sizeof(T) <= 2;
};

TEST_CASE("filter") {
    using list = typelist<int, char, float, double>;
    static_assert(std::is_same_v<filter_t<list, check_size>::type,
                                 typelist<char>>);
}

//
// fold
//

template<typename T>
struct self_type {
    using type = T;
};

template<is_typelist In, typename I, template<typename, typename> typename Op>
struct fold : self_type<I> {};

template<typename I, template<typename, typename> typename Op, typename H, typename... Ts>
struct fold<typelist<H, Ts...>, I, Op> : fold<typelist<Ts...>, typename Op<I, H>::type, Op> {};

template<is_typelist In, typename I, template<typename, typename> typename Op>
using fold_t = typename fold<In, I, Op>::type;

template<typename Acc, typename T>
using type_size_acc = std::integral_constant<std::size_t, Acc::value + sizeof(T)>;

TEST_CASE("fold") {
    using list = typelist<int, char, float, double>;
    static_assert(
            fold_t<list, std::integral_constant<size_t, 0>, type_size_acc>::value == 4 + 1 + 4 + 8);
}

int main(int argc, const char* argv[]) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    return context.run();
}
