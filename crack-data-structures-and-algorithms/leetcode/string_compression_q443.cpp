#include <algorithm>
#include <string>
#include <vector>

using namespace std;

// 核心思路
// 题目不光要原地修改数组，还要返回最后的length；并且 followup 要求 O(1) 额外空间
// 核心框架和 remove-duplicates 类似：如果当前元素和上一个一样，就自增cum
// 如果不一样，那么就先处理上一个元素的累计结果，再从当前元素开始算
// 维护一个 out_it 指向要写入结果的 begin iterator
// 注：不要使用哨兵值来处理最后的元素，因为你不知道用来做哨兵的值会不会出现在内容里
class Solution {
public:
    int compress(vector<char>& chars) {
        int count = 0;
        auto out_it = chars.begin();
        for (auto i = 0; i < chars.size(); ++i) {
            if (i > 0 && chars[i] != chars[i-1]) {
                *out_it++ = chars[i-1];
                if (count > 1) {
                    auto s = to_string(count);
                    out_it = copy(s.begin(), s.end(), out_it);
                }

                count = 1;
            } else {
                ++count;
            }
        }

        *out_it++ = chars.back();
        if (count > 1) {
            auto s = to_string(count);
            out_it = copy(s.begin(), s.end(), out_it);
        }

        return distance(chars.begin(), out_it);
    }
};
