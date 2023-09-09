/*
 @ Kingsley Chen
*/

#include "buddy_allocator.h"

#include "kbase/error_exception_util.h"

namespace {

using pointer = BuddyAllocator::pointer;
using size_type = BuddyAllocator::size_type;

// In strict mode, this function throws an exception if |size_in_bytes| is not a
// multiple of slot granularity.
// Otherwise, it returns ceil(size_in_bytes / granularity).
size_t BytesToSlots(size_t size_in_bytes, bool strict_mode)
{
    auto granularity = BuddyAllocator::granularity();
    if (strict_mode) {
        ENSURE(RAISE, size_in_bytes % granularity == 0)(size_in_bytes).Require();
    }

    return (size_in_bytes + granularity - 1) / granularity;
}

}   // namespace

// static
size_t BuddyAllocator::granularity()
{
    using SlotType = decltype(memory_bin_);
    return SlotType::slot_granularity;
}

BuddyAllocator::BuddyAllocator(size_type max_byte /* = DEFAULT_CAPACITY */)
    : bin_manager_(BytesToSlots(max_byte, true)),
      memory_bin_(BytesToSlots(max_byte, true)),
      capacity_(max_byte)
{}

pointer BuddyAllocator::Allocate(size_type number_of_bytes,
                                 size_type* bytes_allocated)
{
    size_t slots_required = BytesToSlots(number_of_bytes, false);

    size_t offset = bin_manager_.Allocate(slots_required);
    if (offset == BinManager::noffset) {
        return nullptr;
    }

    if (bytes_allocated) {
        *bytes_allocated =
            bin_manager_.SlotCountFor(offset) * BuddyAllocator::granularity();
    }

    pointer addr = static_cast<pointer>(memory_bin_.Map(offset));
    AddAllocationRecord(addr, offset);

    return addr;
}

void BuddyAllocator::Deallocate(pointer& ptr)
{
    ptr_int raw_ptr = reinterpret_cast<ptr_int>(ptr);
    auto iter = allocation_table_.find(raw_ptr);
    if (iter == allocation_table_.end()) {
        return;
    }

    bin_manager_.Free(iter->second.second);
    allocation_table_.erase(iter);

    ptr = nullptr;
}

size_type BuddyAllocator::QueryAllocatedSize(pointer ptr)
{
    ptr_int raw_ptr = reinterpret_cast<ptr_int>(ptr);
    auto iter = allocation_table_.find(raw_ptr);
    if (iter == allocation_table_.end()) {
        return 0;
    }

    size_t slot_count = iter->second.first;

    return slot_count * BuddyAllocator::granularity();
}

void BuddyAllocator::AddAllocationRecord(pointer ptr, size_t offset)
{
    ptr_int raw_ptr = reinterpret_cast<ptr_int>(ptr);
    size_t slot_count = bin_manager_.SlotCountFor(offset);

    allocation_table_[raw_ptr] = std::make_pair(slot_count, offset);
}