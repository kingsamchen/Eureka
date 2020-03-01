#include <random>
#include <vector>

using namespace std;

// 核心思路
// 其实就是 Fisher-yates shuffling algorithm
class Solution {
public:
    vector<int> nums_;
    default_random_engine engine_;
    Solution(vector<int>& nums)
        : nums_(nums), engine_(random_device{}())
    {
    }

    /** Resets the array to its original configuration and return it. */
    vector<int> reset() {
        return nums_;
    }

    /** Returns a random shuffling of the array. */
    vector<int> shuffle() {
        auto result = nums_;
        for (auto i = 0; i < static_cast<int>(nums_.size()) - 1; ++i) {
            uniform_int_distribution<int> dist(i, nums_.size() - 1);
            auto target = dist(engine_);
            swap(result[i], result[target]);
        }

        return result;
    }
};
