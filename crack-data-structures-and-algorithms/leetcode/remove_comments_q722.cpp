#include <string>
#include <sstream>
#include <vector>

using namespace std;

// 核心思路
// 首先用 join('\n') 连成完整的代码字符串，然后
// 1) 从头开始遍历每个字符，并且检查当前字符是不是 // 或者 /*
// 2) 如果遇到 //，那么就一直找到最近的 \n，然后删掉 // 开始到 \n 前一个字符；字符串长度会减少
// 3）如果遇到 /*，那就找到最近的一个 */，然后删除这部分注释；同样的，会引起字符串减少
// 对于非注释或者 // 的case，pos游标需要自增；对于块注释，因为删除了整块，游标不动
// 然后剩下的就是对\n做split
class Solution {
public:
    vector<string> removeComments(vector<string>& source) {
        string code;
        for (const auto& s : source) {
            code.append(s).append("\n");
        }

        for (auto pos = 0; pos < code.size();) {
            if (code[pos] == '/' && pos + 1 < code.size()) {
                if (code[pos+1] == '/') {
                    auto line_end = code.find("\n", pos);
                    code.erase(pos, line_end - pos);
                    // pos now points to \n
                } else if (code[pos+1] == '*') {
                    auto block_end = code.find("*/", pos + 2);
                    code.erase(pos, block_end - pos + 2);
                    continue;
                }
            }

            ++pos;
        }

        istringstream in(code);
        string line;
        vector<string> result;
        while (getline(in, line, '\n')) {
            if (!line.empty()) {
                result.push_back(line);
            }
        }

        return result;
    }
};
