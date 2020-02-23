#include <deque>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

// 核心思路
// 使用BFS，对每一轮的每个单词进行一次一个字符的替换
// 如果新单词在字典中，那么就加入进入下一轮的candidates队列，并且从字典中删除这个新单词（避免重复）
// 每一轮结束后，round自增；注意，每一轮会处理这一轮的所有单词后选
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set dict(wordList.begin(), wordList.end());

        // Not a chance.
        if (dict.count(endWord) == 0) {
            return 0;
        }

        int round = 0;
        deque<string> round_candidates {beginWord};
        deque<string> round_check;
        while (!round_candidates.empty()) {
            swap(round_check, round_candidates);
            while (!round_check.empty()) {
                auto str = round_check.front();
                round_check.pop_front();
                if (str == endWord) {
                    return round + 1;
                }

                // Try to replace each character of str with new other characters.
                for (auto i = 0; i < str.size(); ++i) {
                    auto new_str = str;
                    for (char c = 'a'; c <= 'z'; ++c) {
                        new_str[i] = c;
                        if (new_str != str && dict.count(new_str) != 0) {
                            round_candidates.push_back(new_str);
                            dict.erase(new_str);
                        }
                    }
                }
            }

            // Try next round for transformation.
            ++round;
        }

        return 0;
    }
};