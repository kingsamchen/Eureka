/*
 @ 0xCCCCCCCC
*/

#pragma once

template<typename T, size_t N>
class array_result {
public:
    using iterator = T*;
    using const_iterator = const T*;

    constexpr T& operator[](size_t i)
    {
        return ary_[i];
    }

    constexpr const T& operator[](size_t i) const
    {
        return ary_[i];
    }

    constexpr iterator begin()
    {
        return &ary_[0];
    }

    constexpr iterator end()
    {
        return &ary_[N];
    }

    constexpr const_iterator cbegin() const
    {
        return &ary_[0];
    }

    constexpr const_iterator cend() const
    {
        return &ary_[size_];
    }

    constexpr size_t size() const
    {
        return size_;
    }

private:
    T ary_[N] {};
    static constexpr size_t size_ = N;
};
