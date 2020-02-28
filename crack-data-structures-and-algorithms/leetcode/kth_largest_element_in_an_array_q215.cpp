# include <vector>

using namespace std;

// 核心思路
// 将 kth larget 转换为 kth smallest
// 然后就可以应用算法导论上经典的 quick-selection 算法
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        return Select(nums, 0, nums.size() - 1, nums.size() + 1 - k);
    }

    int Select(vector<int>& nums, int left, int right, int ks)
    {
        if (left == right) {
            return nums[left];
        }

        auto pivot = nums[right];
        auto l = left - 1;
        for (auto i = left; i < right; ++i) {
            if (nums[i] <= pivot) {
                ++l;
                swap(nums[l], nums[i]);
            }
        }

        ++l;
        swap(nums[l], nums[right]);

        auto pi = l;
        auto lcnt = pi - left + 1;
        if (lcnt == ks) {
            return nums[pi];
        }

        if (ks < lcnt) {
            return Select(nums, left, pi - 1, ks);
        } else {
            return Select(nums, pi + 1, right, ks - lcnt);
        }
    }
};
