/*
 @ Kingsley Chen
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef MEMORY_BIN_H_
#define MEMORY_BIN_H_

#include <cstdint>
#include <memory>

#include "kbase/error_exception_util.h"

#include "buddy_util.h"
#include "compiler_helper.h"

template<typename BlockType, size_t Granularity>
struct SlotTraits {
    enum {
        granularity = Granularity,
        block_count = granularity / sizeof(BlockType)
    };

    BlockType s[block_count];
};

// Using 4K for a slot by default.
using DefaultSlotTraits = SlotTraits<uint8_t, 4096>;

template<typename SlotTraitsType = DefaultSlotTraits>
class MemoryBin {
public:
    using Slot = SlotTraitsType;

    // |slot_count| must be a power of 2.
    explicit MemoryBin(size_t slot_count)
        : slot_count_(slot_count)
    {
        ENSURE(RAISE, buddy_util::IsPowerOf2(slot_count))(slot_count).Require();
        slots_ = std::make_unique<Slot[]>(slot_count);
    }

    MemoryBin(const MemoryBin&) = delete;

    MemoryBin(MemoryBin&& other)
        : slots_(std::move(other.slots_)), slot_count_(other.slot_count_)
    {
        other.slot_count_ = 0;
    }

    MemoryBin& operator=(const MemoryBin&) = delete;

    MemoryBin& operator=(MemoryBin&& rhs)
    {
        if (this != &rhs) {
            slots_ = std::move(rhs.slots_);
            slot_count_ = rhs.slot_count_;
            rhs.slot_count_ = 0;
        }

        return *this;
    }

    ~MemoryBin() = default;

    Slot* Map(size_t offset)
    {
        ENSURE(RAISE, offset < slot_count_)(offset)(slot_count_).Require();
        return &slots_[offset];
    }

    size_t slot_count() const
    {
        return slot_count_;
    }

    const static size_t slot_granularity = Slot::granularity;

private:
    FRIEND_TEST_WITH_PREFIX(MemoryBin);

private:
    std::unique_ptr<Slot[]> slots_;
    size_t slot_count_;
};

#endif  // MEMORY_BIN_H_