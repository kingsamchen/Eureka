#include <string>
#include <vector>

using namespace std;

// 核心思路
// lpn, rpn 分别表示当前还剩下几个(, ) 可以使用
// 使用递归回溯来生成组合
// 1) lpn == 0 && rpn == 0 表明所有括号都已经使用，出现了一个组合结果
// 2) lpn > rpn 时说明当前的组合不是 well-formed 的，及时结束
// 3) 然后分别尝试用了一个(，以及用了一个)的情况往下递归；因为要求 lpn <= rpn，所以生成(时要满足lpn > 0
// 针对一个 n = 2 的调用图
//                  (2, 2)
//                  /    \
//              (1, 2)  (2, 1)  X, not well-formed
//              /    \
//          (0, 2)   (1, 1)
//            |        |   \
//          (0, 1)   (0, 1)  (1, 0) X，not well-formed
//            /           \
//         (0, 0)         (0, 0)
//           ||             ||
//    =>    (())           ()()
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> result;
        generate(n, n, string(), result);
        return result;
    }

    void generate(int lpn, int rpn, string combi, vector<string>& result) {
        if (lpn == 0 && rpn == 0) {
            result.push_back(combi);
            return;
        }

        if (lpn > rpn) {
            return;
        }

        if (lpn > 0) {
            generate(lpn - 1, rpn, combi + '(', result);
        }

        generate(lpn, rpn - 1, combi + ')', result);
    }
};
