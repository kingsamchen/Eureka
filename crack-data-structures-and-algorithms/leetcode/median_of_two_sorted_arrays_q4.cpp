#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 提供一个 GetKth() 函数返回两个有序数组合并后的第k个元素
// 对于每个数组，求第 k/2 个元素（不要超过数组最大长度）
// 如果 a1[k/2] < a2[k/2]，则说明a1[begin, k/2]都要小于a2[k/2]，那么就算合并了kth元素也不会出现a1的范围里
// 对于 a2[k/2] < a1[k/2] 同理
// 对于 a1[k/2] == a2[k/2]，随便归到那种case，因为反正我们会留下一个相同的元素
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        auto len1 = nums1.size();
        auto len2 = nums2.size();

        // Idioms here.
        auto m1 = (len1 + len2 + 1) / 2;
        auto m2 = (len1 + len2 + 2) / 2;
        auto val1 = GetKth(nums1, 0, len1, nums2, 0, len2, m1);
        auto val2 = GetKth(nums1, 0, len1, nums2, 0, len2, m2);

        return (val1 + val2) / 2.;
    }

    // returns k-th element in an array combined by a1 & a2 provided still being sorted.
    // range: [begin, end)
    int GetKth(const vector<int>& a1, int begin1, int end1, const vector<int>& a2, int begin2, int end2, int k)
    {
        auto len1 = end1 - begin1;
        auto len2 = end2 - begin2;
        if (len1 < len2) {
            return GetKth(a2, begin2, end2, a1, begin1, end1, k);
        }

        // base case 1
        if (len2 == 0) {
            return a1[begin1+k-1];
        }

        // base case 2
        if (k == 1) {
            return min(a1[begin1], a2[begin2]);
        }

        auto m1 = begin1 + min(len1, k / 2) - 1;
        auto m2 = begin2 + min(len2, k / 2) - 1;

        // Doesn't matter how to handle the equality case.
        // Because we can eliminate either one of two.
        if (a1[m1] < a2[m2]) {
            return GetKth(a1, m1 + 1, end1, a2, begin2, end2, k - (m1 - begin1 + 1));
        } else {
            return GetKth(a1, begin1, end1, a2, m2 + 1, end2, k - (m2 - begin2 + 1));
        }
    }
};
