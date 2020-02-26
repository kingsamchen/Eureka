#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 题目不光要原地修改数组，还要返回最后的length；并且 followup 要求 O(1) 额外空间
// 核心框架和 remove-duplicates 类似：如果当前元素和上一个一样，就自增cum
// 如果不一样，那么就先处理上一个元素的累计结果，再从当前元素开始算
// 遍历从 a[1] 开始处理，所以一开始有 cum=1 并且 ch=a[0]
// 维护一个 out_it 指向要写入结果的 begin iterator
class Solution {
public:
    int compress(vector<char>& chars) {
        if (chars.size() < 2) {
            return chars.size();
        }

        int total = 0;
        int cum = 1;
        char ch = chars[0];
        auto out_it = chars.begin();
        for (auto i = 1; i < chars.size(); ++i) {
            if (chars[i] == chars[i-1]) {
                ++cum;
            } else {
                out_it = WriteCompressed(out_it, ch, cum);
                total += GetCompressedCount(cum);
                cum = 1;
                ch = chars[i];
            }
        }

        out_it = WriteCompressed(out_it, ch, cum);
        total += GetCompressedCount(cum);

        return total;
    }

    template<typename ITER>
    auto WriteCompressed(ITER out, char ch, int cum)->ITER
    {
        *out++ = ch;
        if (cum == 1) {
            return out;
        }

        auto s = to_string(cum);
        return copy(s.begin(), s.end(), out);
    }

    int GetCompressedCount(int raw)
    {
        if (raw == 1) {
            return 1;
        }

        if (2 <= raw && raw <= 9) {
            return 2;
        }

        if (10 <= raw && raw <= 99) {
            return 3;
        }

        if (100 <= raw && raw <= 999) {
            return 4;
        }

        return 5;
    }
};
