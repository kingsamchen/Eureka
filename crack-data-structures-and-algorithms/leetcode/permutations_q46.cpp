#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 首先 sort 后保证元素按非递减序，这是我们第一个排列
// 然后不断调用 next_permu 来生成下一个排列
// next_permu 核心思路
// 1) 找到**最大**的一个 i 满足 arr[i] < arr[i+1]，注意这里 i 范围为 [0, lens-2]
// 如果找不到这样的 i 说明当前已经是最后一个排列！
// 2) 假设第一步能找到，这一步找到**最大**的一个 j 使得 arr[i] < arr[j]；这里 j 范围为 [0, len-1]
// 3) swap arr[i] <-> arr[j]
// 4) reverse arr[i+1...len-1] 即反转 i+1 开始到末尾的所有元素
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> result;
        do {
            result.push_back(nums);
        } while (next_permu(nums));

        return result;
    }

    bool next_permu(vector<int>& nums) {
        int l = -1;
        for (int i = 0; i < nums.size() - 1; ++i) {
            if (nums[i] < nums[i+1]) {
                l = i;
            }
        }

        if (l == -1) {
            return false;
        }

        int r = -1;
        for (int i = l + 1; i < nums.size(); ++i) {
            if (nums[l] < nums[i]) {
                r = i;
            }
        }

        swap(nums[l],nums[r]);
        reverse(next(nums.begin(), l + 1), nums.end());
        return true;
    }
};
