#include <algorithm>
#include <string>
#include <vector>

using namespace std;

// 核心思路
// 递归减治：对于每个segment，长度可能在1~3，每次尝试划分出一个segment，看是否符合ip数字段要求
// 如果就先认为有效，然后不断递归处理
// 需要注意有效性判断；同时每段的长度准确为[1, min(3, L-idx)]，L-idx表示当前还未遍历过的字符串长度
// 因为可能出现四个segment都配齐但是字符串尚未遍历完毕的情况，所以结束条件中，必须要
// done == 4 && idx == s.size() 才可以认为这个结果是OK的。
class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        std::vector<std::string> ips;
        DoRestore(s, 0, 0, std::string(), ips);
        return ips;
    }

    void DoRestore(const std::string& s, int idx, int done, std::string ip, std::vector<std::string>& result)
    {
        if (done == 4) {
            if (idx == s.size()) {
                ip.pop_back();
                result.push_back(std::move(ip));
            }
            return;
        }

        auto len = min<int>(3, s.size() - idx);
        for (auto i = 1; i <= len; ++i) {
            auto seg = s.substr(idx, i);
            if (seg.size() > 1 && seg[0] == '0') {
                continue;
            }

            auto digits = stoi(seg);
            if (0 <= digits && digits <= 255) {
                DoRestore(s, idx + i, done + 1, ip + seg + ".", result);
            }
        }
    }
};
