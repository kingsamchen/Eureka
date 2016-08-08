/*
 @ 0xCCCCCCC
*/

#pragma once

#include <iosfwd>

class ThreadSafeStringData;

class ThreadSafeRCString {
public:
    ThreadSafeRCString();

    explicit ThreadSafeRCString(const char* str);

    ThreadSafeRCString(const char* str, size_t length);

    ThreadSafeRCString(const ThreadSafeRCString& other);

    ~ThreadSafeRCString();

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
    ThreadSafeStringData* data_;
};

std::ostream& operator<< (std::ostream& os, const ThreadSafeRCString& str);