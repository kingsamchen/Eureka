#include <algorithm>
#include <string>
#include <vector>

using namespace std;

// 核心思路
// 递归减治：对于每个segment，长度可能在1~3，每次尝试划分出一个segment，看是否符合ip数字段要求
// 如果就先认为有效，然后不断递归处理.
// 首先对每个segment应当由 start + i <= s.size(), 1 <= i <= 3，然后
// 如果当前segment长度大于1，那么要检查leading应该不为0；同时如果segment长度为3，要检查数值上不能大于255
// 因为可能出现四个segment都配齐但是字符串尚未遍历完毕的情况，所以结束条件中，必须要
// done == 4 && idx == s.size() 才可以认为这个结果是OK的。
class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> ips;
        restore(s, 0, 0, string(), ips);
        return ips;
    }

    void restore(const string& s, int start, int done, string ip, vector<string>& result) {
        if (done == 4) {
            if (start == s.size()) {
                ip.pop_back();
                result.push_back(ip);
            }
            return;
        }

        for (auto i = 1; i <= 3 && start + i <= s.size(); ++i) {
            if (i > 1 && s[start] == '0') {
                continue;
            }

            if (i == 3 && stoi(s.substr(start, 3)) > 255) {
                continue;
            }

            restore(s, start + i, done + 1, ip + s.substr(start, i) + ".", result);
        }
    }
};
