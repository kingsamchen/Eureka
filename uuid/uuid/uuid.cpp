#include "uuid/uuid.h"

#include <cstdio>

namespace {

constexpr size_t k_canonical_len = 36;

}

namespace uuid {

// -*- default_random_generator -*-

default_random_generator& default_random_generator::instance()
{
    static default_random_generator instance;
    return instance;
}

default_random_generator::default_random_generator()
    : engine_(std::random_device{}())
{}

uint64_t default_random_generator::operator()()
{
    std::lock_guard guard(mtx_);
    return engine_();
}

// -*- uuid -*-

void uuid::set_version(uuid::version ver) noexcept
{
    auto ver_bits = static_cast<uint64_t>(ver) << 12;
    data_[0] &= 0xffff'ffff'ffff'0fffull;
    data_[0] |= ver_bits;
}

void uuid::set_variant() noexcept
{
    data_[1] &= 0x3fff'ffff'ffff'ffffull;
    data_[1] |= 0x8000'0000'0000'0000ull;
}

std::string uuid::string() const
{
    std::string s(k_canonical_len + 1, 0);
    snprintf(s.data(), s.size(), "%08llx-%04llx-%04llx-%04llx-%012llx",
             data_[0] >> 32, (data_[0] >> 16) & 0xffffull, data_[0] & 0xffffull,
             data_[1] >> 48, data_[1] & 0xff'ffff'ffffull);
    s.resize(k_canonical_len);
    return s;
}

}   // namespace uuid
