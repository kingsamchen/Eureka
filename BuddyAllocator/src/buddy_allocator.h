/*
 @ Kingsley Chen
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef BUDDY_ALLOCATOR_H_
#define BUDDY_ALLOCATOR_H_

#include <unordered_map>

#include "bin_manager.h"
#include "memory_bin.h"

// This class is only a facade for using a combination of memory bin and bin manager
// objects to implement buddy-allocator.
// No specific details are concerned.
class BuddyAllocator {
public:
    using size_type = size_t;
    using pointer = void*;

private:
    using ptr_int = size_t;
    using AllocationInfo = std::pair<size_t, size_t>;    // <bin-size, offset>
    using AllocationTracker = std::unordered_map<ptr_int, AllocationInfo>;

    enum { DEFAULT_CAPACITY = 64 * 1024 };

public:
    // |max_byte| must be a multiple of granularity.
    explicit BuddyAllocator(size_type max_byte = DEFAULT_CAPACITY);

    BuddyAllocator(const BuddyAllocator&) = delete;

    BuddyAllocator& operator=(const BuddyAllocator&) = delete;

    ~BuddyAllocator() = default;

    pointer Allocate(size_type number_of_bytes, size_type* bytes_allocated);

    void Deallocate(pointer& ptr);

    size_type QueryAllocatedSize(pointer ptr);

    size_type capacity() const
    {
        return capacity_;
    }

    // Returns the granularity of underlying slot.
    static size_t granularity();

private:
    void AddAllocationRecord(pointer ptr, size_t offset);

private:
    BinManager bin_manager_;
    MemoryBin<> memory_bin_;
    AllocationTracker allocation_table_;
    size_type capacity_;
};

#endif  // BUDDY_ALLOCATOR_H_