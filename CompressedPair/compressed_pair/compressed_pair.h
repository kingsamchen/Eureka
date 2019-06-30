/*
 @ 0xCCCCCCCC
*/

#ifndef COMPRESSED_PAIR_H_
#define COMPRESSED_PAIR_H_

#include <type_traits>

template<typename Tx, typename Ty, bool = std::is_empty<Tx>::value>
struct CompressedPair : Tx {
    Ty second;

    template<typename T>
    explicit CompressedPair(T&& t)
        : Tx(),
          second(std::forward<T>(t))
    {}

    template<typename T, typename U>
    CompressedPair(T&& t, U&& u)
        : Tx(std::forward<T>(t)),
          second(std::forward<U>(u))
    {}

    Tx& get_first() noexcept
    {
        return *this;
    }

    const Tx& get_first() const noexcept
    {
        return *this;
    }
};

template<typename Tx, typename Ty>
struct CompressedPair<Tx, Ty, false> {
    Tx first;
    Ty second;

    template<typename T, typename U>
    CompressedPair(T&& t, U&& u)
        : first(std::forward<T>(t)),
          second(std::forward<U>(u))
    {}

    Tx& get_first() noexcept
    {
        return first;
    }

    const Tx& get_first() const noexcept
    {
        return first;
    }
};

#endif  // COMPRESSED_PAIR_H_
