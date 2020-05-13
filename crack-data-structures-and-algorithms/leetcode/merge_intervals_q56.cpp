#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 将区间列表按照 interval.first 元素为基准进行排序，STL 的 sort() 刚好已经足够
// 一个result列表保存处理过的不重叠的区间，初始元素是排序后的intervals[0]；注意这个列表的区间也是按照第一个元素有序
// 从第二个元素开始遍历intervals，和result[-1]比较：如果 result[-1].second >= intervals[i].first
// 则说明区间重叠，区间的右半部分是 max(result[-1].second, intervals[i].second)，因为区间可能完全覆盖
// 如果不重叠则将intervals[i]区间加入到result队尾
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if (intervals.size() < 1) {
            return {};
        }

        sort(intervals.begin(), intervals.end());

        vector<vector<int>> result {intervals[0]};
        for (int i = 1; i < intervals.size(); ++i) {
            if (result.back()[1] >= intervals[i][0]) {
                result.back()[1] = max(intervals[i][1], result.back()[1]);
            } else {
                result.push_back(intervals[i]);
            }
        }

        return result;
    }
};
