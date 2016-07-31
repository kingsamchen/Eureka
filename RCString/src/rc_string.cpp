/*
 @ 0xCCCCCCCC
*/

#include "rc_string.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <string>

constexpr size_t kBaseSize = 4;

// Rounds up `num` to the nearest multiple of `factor`.
constexpr size_t RoundToMultiple(size_t num, size_t factor)
{
    return factor == 0 ? 0 : (num - 1 - (num - 1) % factor + factor);
}

class StringData {
public:
    explicit StringData(size_t capacity)
        : size_(0), ref_count_(1)
    {
        Reserve(capacity);
    }

    ~StringData()
    {
        assert(buf_ != nullptr);
        assert(ref_count_ == 0);
        operator delete(buf_);
        std::cout << "[debug]: release StringData\n";
    }

    StringData(const StringData&) = delete;

    StringData& operator=(const StringData&) = delete;

    StringData* Clone(size_t new_capacity) const;

    void CopyData(const char* str, size_t length, size_t pos) noexcept;

    // Ensures enought capacity (at least in size of `capacity`) for string content.
    // Note that this function will never change the content.
    void Reserve(size_t capacity);

    char* data() const noexcept
    {
        return buf_;
    }

    size_t size() const noexcept
    {
        return size_;
    }

    bool Unique() const noexcept
    {
        return ref_count_ == 1 || ref_count_ == kUnsharedRefMark;
    }

    bool Unsharedable() const noexcept
    {
        return ref_count_ == kUnsharedRefMark;
    }

    // If the data is unsharedable, no more owner can share this resource.
    void MakeUnsharedable() const noexcept
    {
        assert(Unique());
        ref_count_ = kUnsharedRefMark;
    }

    void ResetSharedable() const noexcept
    {
        assert(Unique());
        ref_count_ = 1;
    }

    void AddRef() const noexcept
    {
        assert(ref_count_ > 0);
        ++ref_count_;
    }

    // The last owner should cleanup the data when this function returns true.
    bool Release() const noexcept
    {
        assert(ref_count_ > 0);
        if (--ref_count_ == 0) {
            return true;
        }

        return false;
    }

private:
    char* buf_;
    size_t capacity_;
    size_t size_;
    mutable unsigned int ref_count_;
    static constexpr size_t kUnsharedRefMark = static_cast<unsigned int>(-1);
};

void StringData::Reserve(size_t required_capacity)
{
    if (required_capacity <= capacity_) {
        return;
    }

    auto new_base = std::max(capacity_ * 3 / 2, required_capacity);
    auto new_capacity = RoundToMultiple(new_base, kBaseSize);
    char* new_buf = static_cast<char*>(operator new(new_capacity));
    memcpy_s(new_buf, new_capacity, buf_, size_);

    operator delete(buf_);  // operator delete is a no-throw op.
    buf_ = new_buf;
    capacity_ = new_capacity;
}

void StringData::CopyData(const char* str, size_t length, size_t pos) noexcept
{
    assert(Unique());
    assert(pos <= size_);
    assert(pos + length <= capacity_);
    memcpy_s(buf_ + pos, capacity_, str, length);
    size_ = std::max(pos + length, size_);
}

StringData* StringData::Clone(size_t new_capacity) const
{
    auto* data = new StringData(std::max(new_capacity, capacity_));
    data->CopyData(buf_, size_, 0);
    return data;
}

// -*- RCString implementation -*-

RCString::RCString()
    : data_(nullptr)
{}

RCString::RCString(const RCString& other)
{
    data_ = other.data_;
    data_->AddRef();
}

RCString::RCString(const char* str)
    : RCString(str, strlen(str))
{}

RCString::RCString(const char* str, size_t length)
    : data_(new StringData(length))
{
    data_->CopyData(str, length, 0);
}

RCString::~RCString()
{
    std::cout << "[debug]: destroying RCString\n";
    if (data_ && data_->Release()) {
        delete data_;
    }
}

void RCString::PrepareToModify(size_t required_capacity, bool make_unsharedable)
{
    if (!data_->Unique()) {
        auto* new_data = data_->Clone(required_capacity);
        data_->Release();
        data_ = new_data;
    } else {
        data_->Reserve(required_capacity);
    }

    if (make_unsharedable) {
        data_->MakeUnsharedable();
    } else {
        data_->ResetSharedable();
    }
}

void RCString::Append(const char* str)
{
    auto length = std::char_traits<char>::length(str);
    Append(str, length);
}

void RCString::Append(const char* str, size_t length)
{
    auto new_size = data_->size() + length;
    PrepareToModify(new_size, false);
    data_->CopyData(str, length, data_->size());
}

const char* RCString::data() const noexcept
{
    return data_->data();
}

size_t RCString::size() const noexcept
{
    return data_->size();
}

std::ostream& operator<< (std::ostream& os, const RCString& str)
{
    if (!str.empty()) {
        os.write(str.data(), str.size());
    }

    return os;
}