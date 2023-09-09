/*
 @ Kingsley Chen
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef BIN_MANAGER_H_
#define BIN_MANAGER_H_

#include <vector>

#include "compiler_helper.h"

class BinManager {
public:
    // |initial_slot_count| must be a power of 2.
    explicit BinManager(size_t total_slot_count);

    BinManager(const BinManager&) = delete;

    BinManager(BinManager&& other);

    BinManager& operator=(const BinManager&) = delete;

    BinManager& operator=(BinManager&& rhs);

    ~BinManager() = default;

    // Allocates a bin with enough slots.
    // Returns the offset to the underlying raw memory chunk.
    // Manager will internally round |slots_required| to the nearest a power of 2.
    size_t Allocate(size_t slots_required);

    // Reclaims a bin associated with the |offest|.
    void Free(size_t offset);

    // Gets the slot count of a bin associated with the |offset|.
    size_t SlotCountFor(size_t offset) const;

    static const size_t noffset = static_cast<size_t>(-1);

private:
    FRIEND_TEST_WITH_PREFIX(BinManager);

private:
    size_t total_slot_count_;
    std::vector<size_t> max_consecutive_slot_;
};

#endif  // BIN_MANAGER_H_