/*
 @ 0xCCCCCCCC
*/

#include "buffer.h"

#include <cstring>

#include <endian.h>
#include <sys/uio.h>

#include "kbase/error_exception_util.h"

namespace {

inline int16_t HostToNetwork(int16_t n)
{
    return static_cast<int16_t>(htobe16(static_cast<uint16_t>(n)));
}

inline int32_t HostToNetwork(int32_t n)
{
    return static_cast<int32_t>(htobe32(static_cast<uint32_t>(n)));
}

inline int64_t HostToNetwork(int64_t n)
{
    return static_cast<int64_t>(htobe64(static_cast<uint64_t>(n)));
}

inline int16_t NetworkToHost(int16_t n)
{
    return static_cast<int16_t>(be16toh(static_cast<uint16_t>(n)));
}

inline int32_t NetworkToHost(int32_t n)
{
    return static_cast<int32_t>(be32toh(static_cast<uint32_t>(n)));
}

inline int64_t NetworkToHost(int64_t n)
{
    return static_cast<int64_t>(be64toh(static_cast<uint64_t>(n)));
}

}   // namespace

namespace network {

Buffer::Buffer()
    : Buffer(kDefaultInitialSize)
{}

Buffer::Buffer(size_t initial_size)
    : buf_(initial_size + kDefaultPrependSize),
      reader_index_(kDefaultPrependSize),
      writer_index_(kDefaultPrependSize)
{}

void Buffer::Write(const void* data, size_t size)
{
    ReserveWritable(size);
    ENSURE(CHECK, writable_size() >= size)(writable_size())(size).Require();

    memcpy(BeginWrite(), data, size);
    EndWrite(size);
}

void Buffer::Write(int8_t n)
{
    Write(&n, sizeof(n));
}

void Buffer::Write(int16_t n)
{
    auto be = HostToNetwork(n);
    Write(&be, sizeof(be));
}

void Buffer::Write(int32_t n)
{
    auto be = HostToNetwork(n);
    Write(&be, sizeof(be));
}

void Buffer::Write(int64_t n)
{
    auto be = HostToNetwork(n);
    Write(&be, sizeof(be));
}

int8_t Buffer::PeekAsInt8() const
{
    ENSURE(CHECK, readable_size() >= sizeof(int8_t))(readable_size()).Require();
    int8_t n = *Peek();
    return n;
}

int16_t Buffer::PeekAsInt16() const
{
    ENSURE(CHECK, readable_size() >= sizeof(int16_t))(readable_size()).Require();
    int16_t be;
    memcpy(&be, Peek(), sizeof(be));
    return NetworkToHost(be);
}

int32_t Buffer::PeekAsInt32() const
{
    ENSURE(CHECK, readable_size() >= sizeof(int32_t))(readable_size()).Require();
    int32_t be;
    memcpy(&be, Peek(), sizeof(be));
    return NetworkToHost(be);
}

int64_t Buffer::PeekAsInt64() const
{
    ENSURE(CHECK, readable_size() >= sizeof(int64_t))(readable_size()).Require();
    int64_t be;
    memcpy(&be, Peek(), sizeof(be));
    return NetworkToHost(be);
}

void Buffer::Consume(size_t data_size)
{
    ENSURE(CHECK, data_size <= readable_size())(data_size)(readable_size()).Require();
    if (data_size < readable_size()) {
        reader_index_ += data_size;
    } else {
        ConsumeAll();
    }
}

int8_t Buffer::ReadAsInt8()
{
    auto n = PeekAsInt8();
    Consume(sizeof(n));
    return n;
}

int16_t Buffer::ReadAsInt16()
{
    auto n = PeekAsInt16();
    Consume(sizeof(n));
    return n;
}

int32_t Buffer::ReadAsInt32()
{
    auto n = PeekAsInt32();
    Consume(sizeof(n));
    return n;
}

int64_t Buffer::ReadAsInt64()
{
    auto n = PeekAsInt64();
    Consume(sizeof(n));
    return n;
}

std::string Buffer::ReadAsString(size_t length)
{
    ENSURE(CHECK, readable_size() >= length)(readable_size())(length).Require();
    auto b = begin();
    std::string s(b, b + length);
    Consume(length);
    return s;
}

std::string Buffer::ReadAllAsString()
{
    std::string s(begin(), end());
    ConsumeAll();
    return s;
}

void Buffer::Prepend(const void* data, size_t size)
{
    ENSURE(CHECK, prependable_size() >= size)(prependable_size())(size).Require();
    auto start = reader_index_ - size;
    memcpy(buf_.data() + start, data, size);
    reader_index_ -= size;
}

void Buffer::Prepend(int32_t n)
{
    auto be = HostToNetwork(n);
    Prepend(&be, sizeof(be));
}

void Buffer::Prepend(int64_t n)
{
    auto be = HostToNetwork(n);
    Prepend(&be, sizeof(be));
}

void Buffer::ReserveWritable(size_t new_size)
{
    if (writable_size() >= new_size) {
        return;
    }

    if (prependable_size() + writable_size() < kDefaultPrependSize + new_size) {
        buf_.resize(writer_index_ + new_size);
    } else {
        // Ranges may overlap.
        auto data_size = readable_size();
        memmove(buf_.data() + kDefaultPrependSize, buf_.data() + reader_index_, data_size);
        reader_index_ = kDefaultPrependSize;
        writer_index_ = reader_index_ + data_size;
    }
}

char* Buffer::BeginWrite()
{
    return buf_.data() + writer_index_;
}

void Buffer::EndWrite(size_t written_size)
{
    ENSURE(CHECK, writable_size() >= written_size).Require();
    writer_index_ += written_size;
}

ssize_t ReadFD(int fd, Buffer& buf)
{
    constexpr size_t kExtraBufSize = 65535;
    char extra_buf[kExtraBufSize];

    iovec vec[2];
    vec[0].iov_base = buf.BeginWrite();
    vec[0].iov_len = buf.writable_size();
    vec[1].iov_base = extra_buf;
    vec[1].iov_len = kExtraBufSize;

    auto used_vec_cnt = buf.writable_size() < kExtraBufSize ? 2 : 1;
    auto size_read = readv(fd, vec, used_vec_cnt);
    if (size_read < 0) {
        return -1;
    }

    if (size_read <= buf.writable_size()) {
        buf.EndWrite(size_read);
    } else {
        auto writable = buf.writable_size();
        buf.EndWrite(writable);
        buf.Write(extra_buf, size_read - writable);
    }

    return size_read;
}

}   // namespace network
