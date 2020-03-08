#include <string>
#include <vector>

using namespace std;

// 核心思路
// 首先 expr -> RPN，注意这里要支持负数
// 然后对 RPN 进行求解
class Solution {
public:
    int calculate(string s) {
        auto rpn = expr2rpn(s);
        return eval_rpn(rpn);
    }

int op_priority(char op)
{
    switch (op) {
    case '+':
    case '-':
        return 1;

    case '*':
    case '/':
        return 2;

    case '(':
    case ')':
        return 3;

    default:
        return -1;
    }
}

vector<string> expr2rpn(const string& expr)
{
    vector<string> rpn;
    vector<char> op_stack;
    string last_tok;
    bool negative = false;
    for (auto i = 0; i < expr.size();) {
        if (expr[i] == ' ') {
            ++i;
            continue;
        }

        if (expr[i] == '-' && (last_tok.empty() || !isdigit(last_tok.back())) && last_tok != ")") {
            negative = !negative;
            last_tok = "-";
            ++i;
        } else if (isdigit(expr[i])) {
            auto end = i + 1;
            for (; end < expr.size(); ++end) {
                if (!isdigit(expr[end])) {
                    break;
                }
            }

            string val = negative ? "-" : "";
            val += expr.substr(i, end - i);
            rpn.push_back(val);
            i = end;

            negative = false;
            last_tok = val;
        } else {
            auto ch = expr[i];
            if (ch == '(') {
                op_stack.push_back(ch);
            } else if (ch == ')') {
                while (true) {
                    auto op = op_stack.back();
                    op_stack.pop_back();
                    if (op == '(') {
                        break;
                    }

                    rpn.emplace_back(1, op);
                }
            } else {
                while (true) {
                    if (op_stack.empty() || op_stack.back() == '(' ||
                        op_priority(ch) > op_priority(op_stack.back())) {
                        op_stack.push_back(ch);
                        break;
                    }

                    rpn.emplace_back(1, op_stack.back());
                    op_stack.pop_back();
                }
            }

            ++i;

            last_tok = ch;
        }
    }

    for (auto it = op_stack.rbegin(); it != op_stack.rend(); ++it) {
        rpn.emplace_back(1, *it);
    }

    return rpn;
}

int eval_rpn(const vector<string> rpn)
{
    vector<int64_t> val_stack;
    for (const auto& tok : rpn) {
        if (tok == "+") {
            auto rhs = val_stack.back();
            val_stack.pop_back();
            val_stack.back() += rhs;
        } else if (tok == "-") {
            auto rhs = val_stack.back();
            val_stack.pop_back();
            val_stack.back() -= rhs;
        } else if (tok == "*") {
            auto rhs = val_stack.back();
            val_stack.pop_back();
            val_stack.back() *= rhs;
        } else if (tok == "/") {
            auto rhs = val_stack.back();
            val_stack.pop_back();
            val_stack.back() /= rhs;
        } else {
            val_stack.push_back(stoll(tok));
        }
    }

    return val_stack.back();
}

};
