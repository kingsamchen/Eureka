#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// 核心思路
// 将每个str排序之后作为基准，用map保存基准一样的字符串
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> table;

        for (const auto& s : strs) {
            auto ss = s;
            sort(ss.begin(), ss.end());
            table[ss].push_back(s);
        }

        vector<vector<string>> result;
        for (auto& p : table) {
            result.push_back(std::move(p.second));
        }

        return result;
    }
};
