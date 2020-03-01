#include <vector>

using namespace std;

// 核心思路
// 题目保证不会出现不合法的用例，所以
// 战舰只会出现 XXX 这种横着排列的；或者竖着排列的
// 如果一个位置(i,j)是X，并且他的左边(i,j-1)是X或者上边(i-1,j)是X
// 则说明这个X是已经被计数的战舰的一部分，直接跳过
class Solution {
public:
    int countBattleships(vector<vector<char>>& board) {
        if (board.empty()) {
            return 0;
        }

        int rows = board.size();
        int cols = board[0].size();
        int count = 0;
        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < cols; ++j) {
                if (board[i][j] == 'X') {
                    if ((i > 0 && board[i-1][j] == 'X') ||
                        (j > 0 && board[i][j-1] == 'X'))  {
                        continue;
                    }
                    ++count;
                }
            }
        }

        return count;
    }
};
