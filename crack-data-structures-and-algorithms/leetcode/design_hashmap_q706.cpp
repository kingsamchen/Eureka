#include <algorithm>
#include <list>
#include <vector>

using namespace std;

// 核心思路
// 使用最标准的 hashmap 实现，使用 chain-bucket；表长度固定选一个素数
// 不用考虑 rehash 问题
class MyHashMap {
public:
    using slot = list<pair<int, int>>;
    vector<slot> slots_;
    static constexpr const int kSize = 131;
    /** Initialize your data structure here. */
    MyHashMap()
        : slots_(kSize)
    {}

    /** value will always be non-negative. */
    void put(int key, int value) {
        auto slot_id = key % kSize;
        auto& slot = slots_[slot_id];
        auto it = find_if(slot.begin(), slot.end(), [key](const auto& p) { return p.first == key; });
        if (it != slot.end()) {
            it->second = value;
        } else {
            slot.emplace_back(key, value);
        }
    }

    /** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
    int get(int key) {
        auto slot_id = key % kSize;
        auto& slot = slots_[slot_id];
        auto it = find_if(slot.begin(), slot.end(), [key](const auto& p) { return p.first == key; });
        return it != slot.end() ? it->second : -1;
    }

    /** Removes the mapping of the specified value key if this map contains a mapping for the key */
    void remove(int key) {
        auto slot_id = key % kSize;
        auto& slot = slots_[slot_id];
        slot.remove_if([key](const auto& p) { return p.first == key;});
    }
};
