/*
 @ Kingsley Chen
*/

#include "gtest/gtest.h"

#include <algorithm>
#include <initializer_list>
#include <vector>

#include "bin_manager.h"
#include "buddy_util.h"
#include "memory_bin.h"
#include "buddy_allocator.h"

namespace {

template<typename T>
bool VerifyVector(const std::vector<T>& v, std::initializer_list<T> val_list)
{
    if (v.size() != val_list.size()) {
        return false;
    }

    // Error C4996 received if we were using std::equal.
    auto iv = v.begin();
    auto il = val_list.begin();
    for (; iv != v.end() && il != val_list.end(); ++iv, ++il) {
        if (*iv != *il) return false;
    }

    return true;
}

}   // namespace

TEST(BuddyUtilTest, IsPowerOf2)
{
    size_t power_of_2[] {1, 2, 4, 8, 16};
    size_t not_power_of_2[] {0, 3, 5, 6, 7, 9, 10, 11, 12, 14};

    for (auto ele : power_of_2) {
        EXPECT_TRUE(buddy_util::IsPowerOf2(ele)) << ele << " failed";
    }

    for (auto ele : not_power_of_2) {
        EXPECT_FALSE(buddy_util::IsPowerOf2(ele)) << ele << " failed";
    }
}

TEST(BuddyUtilTest, NearestUpperPowerOf2)
{
    EXPECT_EQ(buddy_util::NearestUpperPowerOf2(1), 1);
    EXPECT_EQ(buddy_util::NearestUpperPowerOf2(4), 4);
    EXPECT_EQ(buddy_util::NearestUpperPowerOf2(7), 8);
}

TEST(BuddyUtilTest, FullBinaryTreeNodeIndexCalculation)
{
    EXPECT_EQ(buddy_util::LeftChild(0), 1);
    EXPECT_EQ(buddy_util::LeftChild(1), 3);
    EXPECT_EQ(buddy_util::LeftChild(2), 5);

    EXPECT_EQ(buddy_util::RightChild(0), 2);
    EXPECT_EQ(buddy_util::RightChild(1), 4);
    EXPECT_EQ(buddy_util::RightChild(2), 6);

    EXPECT_EQ(buddy_util::Parent(1), 0);
    EXPECT_EQ(buddy_util::Parent(2), 0);
    EXPECT_EQ(buddy_util::Parent(4), 1);
    EXPECT_EQ(buddy_util::Parent(5), 2);
}

// ----------------------------------------------------------------------------------

class BinManagerTest : public ::testing::Test {
protected:
    size_t get_total_slot_count(const BinManager& bin_manager)
    {
        return bin_manager.total_slot_count_;
    }

    const std::vector<size_t>& get_maximum_slot(const BinManager& bin_manager)
    {
        return bin_manager.max_consecutive_slot_;
    }
};

TEST_F(BinManagerTest, Construction)
{
    {
        BinManager bm(1);
        ASSERT_EQ(get_total_slot_count(bm), 1);
        EXPECT_TRUE(VerifyVector<size_t>(get_maximum_slot(bm), {1}));
    }
    {
        BinManager bm(4);
        ASSERT_EQ(get_total_slot_count(bm), 4);
        EXPECT_TRUE(VerifyVector<size_t>(get_maximum_slot(bm), {4,2,2,1,1,1,1}));
    }
    //{
    //    EXPECT_ANY_THROW({BinManager bm(0);});
    //}
    //{
    //    EXPECT_ANY_THROW({BinManager bm(7);});
    //}
}

TEST_F(BinManagerTest, Allocate)
{
    BinManager bm(8);
    ASSERT_EQ(get_total_slot_count(bm), 8);
    EXPECT_TRUE(VerifyVector<size_t>(get_maximum_slot(bm), {8,4,4,2,2,2,2,1,1,1,1,1,1,1,1}));

    // need 3 slots, allocate 4 slots in fact.
    // offset == 0
    auto offset = bm.Allocate(3);
    ASSERT_EQ(offset, 0);
    EXPECT_TRUE(VerifyVector<size_t>(get_maximum_slot(bm), {4,0,4,2,2,2,2,1,1,1,1,1,1,1,1}));

    // need 2 slots.
    // offset == 4
    offset = bm.Allocate(2);
    ASSERT_EQ(offset, 4);
    EXPECT_TRUE(VerifyVector<size_t>(get_maximum_slot(bm), {2,0,2,2,2,0,2,1,1,1,1,1,1,1,1}));

    // need yet another 2 slots.
    // offset == 6
    offset = bm.Allocate(2);
    ASSERT_EQ(offset, 6);
    EXPECT_TRUE(VerifyVector<size_t>(get_maximum_slot(bm), {0,0,0,2,2,0,0,1,1,1,1,1,1,1,1}));

    // need 1 slot, but oops.
    // offset == noffset
    offset = bm.Allocate(1);
    ASSERT_EQ(offset, BinManager::noffset);
    EXPECT_TRUE(VerifyVector<size_t>(get_maximum_slot(bm), {0,0,0,2,2,0,0,1,1,1,1,1,1,1,1}));
}

TEST_F(BinManagerTest, Free)
{
    // Preparation.
    BinManager bm(8);
    auto ofs1 = bm.Allocate(4);
    ASSERT_NE(ofs1, BinManager::noffset);
    auto ofs2 = bm.Allocate(2);
    ASSERT_NE(ofs2, BinManager::noffset);
    ASSERT_TRUE(VerifyVector<size_t>(get_maximum_slot(bm), {2,0,2,2,2,0,2,1,1,1,1,1,1,1,1}));

    bm.Free(ofs1);
    EXPECT_TRUE(VerifyVector<size_t>(get_maximum_slot(bm), {4,4,2,2,2,0,2,1,1,1,1,1,1,1,1}));

    bm.Free(ofs2);
    EXPECT_TRUE(VerifyVector<size_t>(get_maximum_slot(bm), {8,4,4,2,2,2,2,1,1,1,1,1,1,1,1}));
}

TEST_F(BinManagerTest, BinSlotCount)
{
    BinManager bm(8);
    auto ofst = bm.Allocate(3);
    EXPECT_EQ(bm.SlotCountFor(ofst), 4);
}

// ----------------------------------------------------------------------------------

class MemoryBinTest : public ::testing::Test {

};

TEST_F(MemoryBinTest, SlotGranularity)
{
    EXPECT_EQ(MemoryBin<>::slot_granularity, 4096);
}

TEST_F(MemoryBinTest, SlotCount)
{
    EXPECT_EQ(MemoryBin<>(4).slot_count(), 4);
}

TEST_F(MemoryBinTest, Map)
{
    MemoryBin<> mb(4);
    auto p1 = reinterpret_cast<uint8_t*>(mb.Map(0));
    auto p2 = reinterpret_cast<uint8_t*>(mb.Map(2));
    ptrdiff_t distance = p2 - p1;
    EXPECT_EQ(distance, MemoryBin<>::slot_granularity * 2);
}

TEST_F(MemoryBinTest, Move)
{
    MemoryBin<> mb(4);
    auto p_old = mb.Map(0);
    MemoryBin<> new_mb(std::move(mb));
    auto p_new = new_mb.Map(0);
    EXPECT_EQ(new_mb.slot_count(), 4);
    EXPECT_EQ(mb.slot_count(), 0);
    EXPECT_EQ(p_old, p_new);
    mb = std::move(new_mb);
    EXPECT_EQ(mb.slot_count(), 4);
    EXPECT_EQ(new_mb.slot_count(), 0);
}

// ----------------------------------------------------------------------------------

TEST(BuddyAllocatorTest, UnderlyingSlotGranularity)
{
    auto slot_granularity = BuddyAllocator::granularity();
    std::cout << slot_granularity << std::endl;
    EXPECT_EQ(slot_granularity, 4096);
}

TEST(BuddyAllocatorTest, Construction)
{
    BuddyAllocator allocator(4096 * 2);
    EXPECT_EQ(allocator.capacity(), 4096 * 2);
}

TEST(BuddyAllocatorTest, NormalUsage)
{
    BuddyAllocator allocator;
    size_t allocated_bytes = 0;
    auto addr = allocator.Allocate(64, &allocated_bytes);
    EXPECT_TRUE(addr != nullptr);
    EXPECT_EQ(allocated_bytes, allocator.QueryAllocatedSize(addr));
    strcpy_s(static_cast<char*>(addr), 64, "hello world!");
    EXPECT_STREQ(static_cast<char*>(addr), "hello world!");
}

// ----------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}