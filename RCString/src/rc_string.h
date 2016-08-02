/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <iostream>

class StringData;

class RCString {
public:
    RCString();

    explicit RCString(const char* str);

    RCString(const char* str, size_t length);

    RCString(const RCString& other);

    ~RCString();

    bool empty() const noexcept
    {
        return size() == 0;
    }

    size_t size() const noexcept;

    const char* data() const noexcept;

    void Append(const char* str);

    void Append(const char* str, size_t length);

    const char& operator[](size_t pos) const;

    char& operator[](size_t pos);

private:
    // For every mutable operations, make the underlying data **unique** first.
    // Note that performing some operations may make the underlying data unsharedable.
    void PrepareToModify(size_t required_capacity, bool make_unsharedable);

private:
    StringData* data_;
};

std::ostream& operator<< (std::ostream& os, const RCString& str);