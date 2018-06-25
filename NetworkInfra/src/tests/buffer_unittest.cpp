
#include "gtest/gtest.h"

#include "buffer.h"

#include <algorithm>

namespace {

constexpr size_t kSupposedPrepend = 8;
constexpr size_t kSupposedInitialSize = 1024;

}   // namespace

namespace network {

TEST(BufferTest, Ctor)
{
    Buffer buf;
    EXPECT_EQ(kSupposedPrepend, buf.prependable_size());
    EXPECT_EQ(0, buf.readable_size());
    EXPECT_EQ(kSupposedInitialSize, buf.writable_size());

    Buffer empty_buf(0);
    EXPECT_EQ(kSupposedPrepend, empty_buf.prependable_size());
    EXPECT_EQ(0, empty_buf.readable_size());
    EXPECT_EQ(0, empty_buf.writable_size());

    Buffer another_buf(32);
    EXPECT_EQ(kSupposedPrepend, another_buf.prependable_size());
    EXPECT_EQ(32, another_buf.writable_size());
}

TEST(BufferTest, GeneralWrite)
{
    // 8 prependable size + 16 initial writable size.
    Buffer buf(16);
    ASSERT_EQ(8, buf.prependable_size());
    ASSERT_EQ(16, buf.writable_size());

    buf.Write("a", 1);
    EXPECT_EQ(1, buf.readable_size());
    EXPECT_EQ(15, buf.writable_size());

    unsigned short s = 0xFFFF;
    buf.Write(&s, sizeof(s));
    EXPECT_EQ(3, buf.readable_size());
    EXPECT_EQ(13, buf.writable_size());

    unsigned int i = 0xDEADBEEF;
    buf.Write(&i, sizeof(i));
    EXPECT_EQ(7, buf.readable_size());
    EXPECT_EQ(9, buf.writable_size());

    buf.Write("ABCD", 4);
    EXPECT_EQ(11, buf.readable_size());
    EXPECT_EQ(5, buf.writable_size());
    EXPECT_EQ(kSupposedPrepend, buf.prependable_size());
}

TEST(BufferTest, Iterators)
{
    Buffer buf;

    ASSERT_EQ(0, buf.readable_size());
    EXPECT_TRUE(buf.begin() == buf.end());

    std::string s("hello world");
    buf.Write(s.data(), s.length());
    EXPECT_FALSE(buf.begin() == buf.end());
    EXPECT_EQ(std::distance(buf.begin(), buf.end()), buf.readable_size());

    auto space_it = std::find(buf.begin(), buf.end(), ' ');
    ASSERT_TRUE(space_it != buf.end());
    EXPECT_EQ(' ', *space_it);
    EXPECT_EQ('w', *std::next(space_it));
    EXPECT_EQ(std::string(buf.begin(), space_it), "hello");
}

TEST(BufferTest, Consumes)
{
    Buffer buf;

    unsigned int magic = 0xDEADBEEF;
    buf.Write(&magic, sizeof(magic));

    std::string s("hello world");
    size_t len = s.length();
    buf.Write(&len, sizeof(len));
    buf.Write(s.data(), len);

    size_t written_size = sizeof(magic) + sizeof(len) + len;
    EXPECT_EQ(written_size, buf.readable_size());

    auto uptr = reinterpret_cast<const unsigned int*>(&(*buf.begin()));
    EXPECT_EQ(0xDEADBEEF, *uptr);
    buf.Consume(sizeof(magic));

    auto len_ptr = reinterpret_cast<const size_t*>(&(*buf.begin()));
    EXPECT_EQ(len, *len_ptr);
    buf.Consume(sizeof(len));

    EXPECT_EQ(kSupposedPrepend + written_size - len, buf.prependable_size());

    EXPECT_EQ(s, std::string(buf.begin(), std::next(buf.begin(), len)));
    buf.Consume(len);
    EXPECT_EQ(0, buf.readable_size());
    EXPECT_EQ(kSupposedPrepend, buf.prependable_size());
}

TEST(BufferTest, WritesAndPeeks)
{
    Buffer buf;
    buf.Write(int8_t(0xFF));
    buf.Write(int16_t(-123));
    buf.Write(int32_t(0xDEADBEEF));
    buf.Write(int64_t(-1));
    auto written_size = sizeof(int8_t) + sizeof(int16_t) + sizeof(int32_t) + sizeof(int64_t);
    EXPECT_EQ(written_size, buf.readable_size());

    EXPECT_EQ(0xFF, static_cast<uint8_t>(buf.PeekAsInt8()));
    buf.Consume(sizeof(int8_t));

    EXPECT_EQ(-123, buf.PeekAsInt16());
    buf.Consume(sizeof(int16_t));

    EXPECT_EQ(0xDEADBEEF, static_cast<unsigned>(buf.PeekAsInt32()));
    buf.Consume(sizeof(int32_t));

    EXPECT_EQ(-1, buf.PeekAsInt64());
}

TEST(BufferTest, Reads)
{
    Buffer buf;
    std::string s("hello world");
    buf.Write(static_cast<int32_t>(s.length()));
    buf.Write(s.data(), s.size());

    auto length = static_cast<size_t>(buf.ReadAsInt32());
    auto str = buf.ReadAllAsString();
    EXPECT_EQ(length, str.length());
    EXPECT_EQ(s, str);
}

TEST(BufferTest, ReserveWritable)
{
    Buffer buf(16);
    std::string s("hello world");
    buf.Write(s.data(), s.size());
    auto it = std::find(buf.begin(), buf.end(), ' ');
    auto partial = buf.ReadAsString(std::distance(buf.begin(), ++it));
    EXPECT_EQ("hello ", partial);
    EXPECT_EQ(16 - s.size(), buf.writable_size());
    EXPECT_EQ(kSupposedPrepend + partial.size(), buf.prependable_size());

    std::string tail(" is flat");
    ASSERT_TRUE(tail.size() > buf.writable_size());
    buf.Write(tail.data(), tail.size());
    EXPECT_EQ(kSupposedPrepend, buf.prependable_size());
    EXPECT_EQ(16 - strlen("world is flat"), buf.writable_size());

    // We have writable-size = 3 now
    std::string ss("; the world has changed");
    buf.Write(ss.data(), ss.size());
    EXPECT_EQ(0, buf.writable_size());
    auto final = buf.ReadAllAsString();
    EXPECT_EQ("world is flat" + ss, final);
    EXPECT_EQ(0, buf.readable_size());
}

TEST(BufferTest, Prepend)
{
    Buffer buf;
    std::string s1("Bilibili gaga");
    buf.Write(s1.data(), s1.size());
    std::string s2("Deep Dark Fantasy");
    buf.Write(s2.data(), s2.size());
    buf.Prepend(static_cast<int32_t>(buf.readable_size()));

    EXPECT_EQ(4, buf.prependable_size());
    auto data_size = buf.ReadAsInt32();
    auto str = buf.ReadAsString(data_size);
    EXPECT_EQ(0, buf.readable_size());
    EXPECT_EQ(data_size, str.size());
    EXPECT_EQ(s1 + s2, str);
}

}   // namespace network
