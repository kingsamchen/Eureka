#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 双指针两边扫描探测法
// 使用一个变量 max_area 来记录探测过程中出现的最大面积
// 因为双指针靠拢的过程中宽减少了，所以我们需要尽可能地让下一次的高变长，才有可能增加面积
class Solution {
public:
    int maxArea(vector<int>& height) {
        int max_area = 0;
        int l = 0, r = height.size() - 1;
        while (l < r) {
            auto area = (r - l) * min(height[l], height[r]);
            max_area = max(max_area, area);
            if (height[l] < height[r]) {
                ++l;
            } else {
                --r;
            }
        }

        return max_area;
    }
};
