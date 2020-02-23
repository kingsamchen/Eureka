#include <algorithm>
#include <bitset>
#include <string>
#include <vector>

using namespace std;

// 核心思路
// 类似于求子集的逻辑，对于每个子字符串，存在使用/不使用两种case，然后递归往下搜索
// 不过这里要使用某个字串首先需要保证这个字串和现有的选择不存在字符重复
class Solution {
public:
    int maxLength(vector<string>& arr) {
        return MaxUniqueLength(arr, 0, 0);
    }

    int MaxUniqueLength(vector<string>& arr, int idx, bitset<26> bits)
    {
        if (idx == arr.size()) {
            return 0;
        }

        const auto& str = arr[idx];
        // Make a copy, for our not-using case.
        auto old_bits = bits;
        for (auto ch : str) {
            if (bits.test(ch - 'a')) {
                // Current string is not allowed, try next one.
                return MaxUniqueLength(arr, idx + 1, old_bits);
            }
            bits.set(ch - 'a');
        }

        int cur_len = str.size();
        return max(cur_len + MaxUniqueLength(arr, idx + 1, bits),
                   MaxUniqueLength(arr, idx + 1, old_bits));
    }
};
