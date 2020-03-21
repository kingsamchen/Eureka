#include <random>
#include <unordered_map>
#include <vector>

using namespace std;

// 核心思路
// 使用一个 hash-table & vector 实现
// hash-table 保存 <value, index> 的组合
// vector直接保存 value，这样可以保证 random 的操作是 O(1)
// 插入新元素时 append 到 vector，保证O(1)
// 移除元素稍微麻烦点，需要将 values[idx] = values[-1]，更新 table[values[-1]] = idx
// 进行偷天换日；然后假装我们要删除的是最后一个元素即可
// 这样三个操作都可以保证 O(1)
class RandomizedSet {
public:
    unordered_map<int, size_t> table_;
    vector<int> values_;
    default_random_engine eng_;

    /** Initialize your data structure here. */
    RandomizedSet()
        : eng_(random_device{}())
    {}

    /** Inserts a value to the set. Returns true if the set did not already contain the specified element. */
    bool insert(int val) {
        if (table_.count(val) == 1) {
            return false;
        }

        table_[val] = values_.size();
        values_.push_back(val);

        return true;
    }

    /** Removes a value from the set. Returns true if the set contained the specified element. */
    bool remove(int val) {
        auto it = table_.find(val);
        if (it == table_.end()) {
            return false;
        }

        auto idx = it->second;
        values_[idx] = values_.back();
        table_[values_.back()] = idx;

        // Erase
        values_.pop_back();
        table_.erase(it);

        return true;
    }

    /** Get a random element from the set. */
    int getRandom() {
        uniform_int_distribution<> dist(0, values_.size() - 1);
        return values_[dist(eng_)];
    }
};
