#include "uuid/uuid.h"

#include <cstdio>

namespace {

constexpr size_t k_canonical_len = 36;

}

namespace uuid {

// implements default_random_generator

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

// implements uuid

void uuid::set_version() noexcept
{
    data_.field_4 &= 0x3fffu;
    data_.field_4 |= 0x8000u;
}

void uuid::set_variant() noexcept
{
    data_.field_3 &= 0x0fffu;
    data_.field_3 |= 0x4000u;
}

std::string uuid::string() const
{
    std::string s(k_canonical_len + 1, 0);
    snprintf(s.data(), s.size(), "%08llx-%04llx-%04llx-%04llx-%012llx",
        data_.field_1, data_.field_2, data_.field_3, data_.field_4, data_.field_5);
    s.resize(k_canonical_len);
    return s;
}

}   // namespace uuid
