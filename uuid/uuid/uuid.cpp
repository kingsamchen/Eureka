#include "uuid/uuid.h"

#include <cstdio>

namespace {

constexpr size_t k_canonical_len = 36;

}

namespace uuid {

// -*- default_random_generator -*-

// static
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

// -*- clock_sequence -*-

// static
clock_sequence& clock_sequence::instance()
{
    static clock_sequence instance;
    return instance;
}

clock_sequence::clock_sequence()
    : last_time_(0),
      seq_(static_cast<uint16_t>(std::random_device{}()))
{}

// static
uint64_t clock_sequence::get_timestamp_since_epoch()
{
    auto ts = k_epoch_diff + std::chrono::duration_cast<duration>(
        std::chrono::system_clock::now().time_since_epoch());
    return ts.count();
}

// TODO: try to replace the mutex with atomic variables.
std::tuple<uint64_t, uint16_t> clock_sequence::read()
{
    auto now = get_timestamp_since_epoch();

    std::lock_guard lock(mtx_);

    // The clock is set backward or read too fast.
    if (now <= last_time_) {
        ++seq_;
    }

    last_time_ = now;

    return std::make_tuple(last_time_, seq_);
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
             data_[1] >> 48, data_[1] & 0xffff'ffff'ffffull);
    s.resize(k_canonical_len);
    return s;
}

}   // namespace uuid
