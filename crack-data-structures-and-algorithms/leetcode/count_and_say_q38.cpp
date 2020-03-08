#include <string>

using namespace std;

// 核心思路
// 题目意思是：当前数是上一个数的描述；例如上一个数 1211 里有1一个1，1个2，2个1
// 那么当前数就是 111221
// 所以用递归加上对上一次数的连续数统计即可
// 需要注意扫描技术后还需要统计最后一个数字
class Solution {
public:
    string countAndSay(int n) {
        if (n == 1) {
            return "1";
        }

        auto prev = countAndSay(n - 1);
        string ans;
        int cnt = 1;
        for (int i = 1; i < prev.size(); ++i) {
            if (prev[i] == prev[i-1]) {
                ++cnt;
                continue;
            }

            ans.append(to_string(cnt)).append(1, prev[i-1]);
            cnt = 1;
        }

        ans.append(to_string(cnt)).append(1, prev.back());

        return ans;
    }
};
