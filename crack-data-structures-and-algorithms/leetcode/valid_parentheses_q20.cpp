#include <string>
#include <vector>

using namespace std;

// 核心思路
// 用一个栈来保存遇到的左括号，即 (, [, {
// 如果遇到一个右括号，此时栈应不为空，并且栈顶是对应的左括号
// 整体遍历结束后，栈为空
class Solution {
public:
    bool isValid(string s) {
        if (s.empty()) {
            return true;
        }

        vector<char> left_pan;
        for (char ch : s) {
            if (ch == '(' || ch == '[' || ch == '{') {
               left_pan.push_back(ch);
               continue;
            }

            auto expected = left_paren(ch);
            if (left_pan.empty() || left_pan.back() != expected) {
                return false;
            }

            left_pan.pop_back();
        }

        return left_pan.empty();
    }

    char left_paren(char right_pan) {
        switch (right_pan) {
            case ')':
                return '(';
            case ']':
                return '[';
            case '}':
            default:
                return '{';
        }
    }
};
