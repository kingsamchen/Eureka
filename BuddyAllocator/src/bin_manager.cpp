/*
 @ Kingsley Chen
*/

#include "bin_manager.h"

#include <algorithm>
#include <cassert>

#include "kbase/error_exception_util.h"

#include "buddy_util.h"


BinManager::BinManager(size_t total_slot_count)
    : total_slot_count_(total_slot_count)
{
    ENSURE(RAISE, buddy_util::IsPowerOf2(total_slot_count))(total_slot_count).Require();

    size_t node_count = (total_slot_count << 1) - 1;
    max_consecutive_slot_.resize(node_count);
    size_t node_slot_count = total_slot_count << 1;
    for (size_t i = 0; i < node_count; ++i) {
        if (buddy_util::IsPowerOf2(i + 1)) {
            node_slot_count >>= 1;
        }

        max_consecutive_slot_[i] = node_slot_count;
    }
}

BinManager::BinManager(BinManager&& other)
    : total_slot_count_(other.total_slot_count_),
      max_consecutive_slot_(std::move(other.max_consecutive_slot_))
{}

BinManager& BinManager::operator=(BinManager&& rhs)
{
    if (this != &rhs) {
        total_slot_count_ = rhs.total_slot_count_;
        max_consecutive_slot_ = std::move(rhs.max_consecutive_slot_);
    }

    return *this;
}

size_t BinManager::Allocate(size_t slots_required)
{
    assert(slots_required != 0);
    if (slots_required == 0) {
        return noffset;
    }

    size_t slots_needed = buddy_util::NearestUpperPowerOf2(slots_required);
    if (max_consecutive_slot_[0] < slots_needed) {
        return noffset;
    }

    size_t index = 0;
    for (auto node_slots = total_slot_count_;
        node_slots != slots_needed;
        node_slots >>= 1) {
        if (max_consecutive_slot_[buddy_util::LeftChild(index)] >= slots_needed) {
            index = buddy_util::LeftChild(index);
        } else {
            index = buddy_util::RightChild(index);
        }
    }

    // The relative distance from first slot in the bin just now allocated to the
    // first slot in the underlying raw memory chunk.
    size_t offset = (index + 1) * slots_needed - total_slot_count_;

    max_consecutive_slot_[index] = 0;
    while (index) {
        index = buddy_util::Parent(index);
        max_consecutive_slot_[index] = std::max(
            max_consecutive_slot_[buddy_util::LeftChild(index)],
            max_consecutive_slot_[buddy_util::RightChild(index)]);
    }

    return offset;
}

void BinManager::Free(size_t offset)
{
    // The value of noffset can be regarded as positive infinity.
    assert(offset < total_slot_count_);
    if (offset >= total_slot_count_) {
        return;
    }

    // Since multiple indices can be mapped into a single offset, we backtrace to
    // locate the bin from bottom.
    // The first bin we meet in the path whose slot count is 0 is the target.
    // Incorrect |offset| may cause undesired deallocation for yet another bin.
    size_t node_slot_count = 1;
    size_t index = offset + total_slot_count_ - 1;
    while (max_consecutive_slot_[index]) {
        // Reached the root. The |offset| is incorret.
        if (index == 0) {
            return;
        }

        node_slot_count <<= 1;
        index = buddy_util::Parent(index);
    }

    max_consecutive_slot_[index] = node_slot_count;
    while (index != 0) {
        index = buddy_util::Parent(index);
        node_slot_count <<= 1;

        size_t lc_slots = max_consecutive_slot_[buddy_util::LeftChild(index)];
        size_t rc_slots = max_consecutive_slot_[buddy_util::RightChild(index)];

        // Merge adjacent bins if possible.
        if (lc_slots + rc_slots == node_slot_count) {
            max_consecutive_slot_[index] = node_slot_count;
        } else {
            max_consecutive_slot_[index] = std::max(lc_slots, rc_slots);
        }
    }
}

size_t BinManager::SlotCountFor(size_t offset) const
{
    assert(offset < total_slot_count_);
    if (offset >= total_slot_count_) {
        return 0;
    }

    size_t node_slot_count = 1;
    for (auto i = offset + total_slot_count_ - 1; max_consecutive_slot_[i];
         i = buddy_util::Parent(i)) {
        node_slot_count <<= 1;
    }

    return node_slot_count;
}