#include <vector>

using namespace std;

// 核心思路
// 同 Q223 Retangle Area 中判断两个举行是否重叠一模一样
// 考虑何种情况下两个矩形不相交，然后取反即可。
// 因为矩形r1、r2可能相对在左边或者右边；上边或者下边，所以注意四个方位都要判断
// 判断核心就是，如果r1相对于r2在上方，那么r1.bottom.y >= r2.top.y
// 如果r1同时相对于r2在左边，那么 r1.top.x < r2.bottom.x
class Solution {
public:
    bool isRectangleOverlap(vector<int>& rec1, vector<int>& rec2) {
        return !((rec2[0] >= rec1[2] || rec2[1] >= rec1[3]) ||
                 (rec1[0] >= rec2[2] || rec1[1] >= rec2[3]));
    }
};
