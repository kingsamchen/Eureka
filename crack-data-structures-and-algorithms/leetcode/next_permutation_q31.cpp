#include <vector>

using namespace std;

// 核心思路
// 标准的 next_permutation 的算法
// 1) 找到一个最大的i，满足 a[i] < a[i+1]
//    如果这一步找不到说明当前序列已经是最后一个 permutation，需要对数组做一次 reverse 转换为第一个
// 2) 从 i+1 开始找到一个最大的 j，满足 a[i] < a[j]；因为(1)满足所以一定能找到这个j
// 3) a[i] <-> a[j]，然后 reverse a[i+1...n)
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int base = -1;
        for (int i = 0; i < nums.size() - 1; ++i) {
            if (nums[i] < nums[i+1]) {
                base = i;
            }
        }

        if (base == -1) {
            reverse(nums.begin(), nums.end());
            return;
        }

        int target = base;
        for (int j = base + 1; j < nums.size(); ++j) {
            if (nums[base] < nums[j]) {
                target = j;
            }
        }

        swap(nums[base], nums[target]);
        reverse(&nums[base+1], &nums.back() + 1);

        return;
    }
};
