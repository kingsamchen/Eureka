#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 利用双指针法两边往中间逼近
// 对于某个坐标，它能储存的容量取决于三个数：最高的一侧higher，储水高度上限upper，和当前高度
// 对于某对l,r (l < r)，higher=max(height[l], height[r]), 当前高度 lower=min(height[l], height[r])
// 在扫描过程中遇到的**最大**的lower形成出水高度上限upper，而出水量=upper-lower
// 指针往higher所在的方向逼近，因为higher的作用是保证储水高度不低于upper
class Solution {
public:
    int trap(vector<int>& height) {
        int volume = 0;
        int l = 0, r = height.size() - 1;
        int upper = 0;
        while (l < r) {
            auto lower = min(height[l], height[r]);
            upper = max(lower, upper);
            volume += upper - lower;
            if (height[l] < height[r]) {
                ++l;
            } else {
                --r;
            }
        }

        return volume;
    }
};
