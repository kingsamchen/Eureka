#include <string>

using namespace std;

// 核心思路
// 假设字符串长度为l，那么统计1、2次数的串结尾此时指向 l-2
// 于是生成对应长度的串再统计即可
class Solution {
public:
    int magicalString(int n) {
        string seq = "12211";
        if (n < 5) {
            return count(seq.begin(), seq.begin() + n, '1');
        }

        for (int p = seq.size() - 1, r = p - 2; p + 1 < n;) {
            int next_cnt = seq[++r] - '0';
            char next_tok = seq[p] == '1' ? '2' : '1';
            seq.append(next_cnt, next_tok);
            p = seq.size() - 1;
        }

        return count(seq.begin(), seq.begin() + n, '1');
    }
};
