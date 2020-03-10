#include <algorithm>
#include <string>
#include <vector>

using namespace std;

// 核心思路
// 将数字转换为字符串，然后排序
// 排序的核心是对于两个字符串x, y如果 xi < yi, 那么 x < y
// 如果 |x| < |y|，那么x遍历结束后一直用x[0]和yi比较
// 如果两个字符串都遍历结束了还不能决出大小，就生成s1=xy s2=yx，比较s1s2大小
// 最后注意全都是0的字符串处理
class Solution {
public:
    string largestNumber(vector<int>& nums) {
        vector<string> ss;
        transform(nums.begin(), nums.end(), back_inserter(ss), [](int val) { return to_string(val); });
        sort(ss.begin(), ss.end(), [](const auto& lhs, const auto& rhs) {
            auto l = 0, r = 0;
            while (l < lhs.size() || r < rhs.size()) {
                auto le = l < lhs.size() ? lhs[l] : lhs.front();
                auto re = r < rhs.size() ? rhs[r] : rhs.front();
                if (le > re) {
                    return true;
                }

                if (le < re) {
                    return false;
                }

                if (l < lhs.size()) {
                    ++l;
                }

                if (r < rhs.size()) {
                    ++r;
                }
            }

            auto s1 = lhs + rhs;
            auto s2 = rhs + lhs;
            return s1 > s2;
        });

        string num;
        for (const auto& s : ss) {
            num += s;
        }

        if (num.size() > 1 && num[0] == '0') {
            num = "0";
        }

        return num;
    }
};
