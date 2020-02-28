#include <bitset>
#include <vector>

using namespace std;

// 核心思路
// 遍历整个9*9矩阵的每个元素，用三组bitset记录出现的数字即可
// rows记录row 0~8；cols记录col 0~8；subs记录9个子数独
// 需要注意的是(i,j)坐标到subs下表的转换
class Solution {
public:
    bool isValidSudoku(vector<vector<char>>& board) {
        bitset<9> rows[9], cols[9], subs[9];
        for (auto i = 0; i < 9; ++i) {
            for (auto j = 0; j < 9; ++j) {
                auto ch = board[i][j];
                if (ch == '.') {
                    continue;
                }

                int val = ch - '1';
                if (rows[i].test(val) || cols[j].test(val) || subs[sub_index(i, j)].test(val)) {
                    return false;
                }

                rows[i].set(val);
                cols[j].set(val);
                subs[sub_index(i, j)].set(val);
            }
        }

        return true;
    }

    inline int sub_index(int i, int j)
    {
        auto r = i / 3;
        auto c = j / 3;
        return 3 * r + c;
    }
};
