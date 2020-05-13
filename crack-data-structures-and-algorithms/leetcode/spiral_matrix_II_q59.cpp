#include <vector>

using namespace std;

// 核心思路
// 遍历思路和 spiral-matrix 这题一样，区别在于遍历的时候按照顺序填充数字
class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> matrix(n, vector<int>(n));

        int xcnt = n;
        int xpos = 1;
        int ycnt = n - 1;
        int ypos = 1;

        int val = 1;
        int row = 0, col = 0;
        while (true) {
            if (xcnt <= 0) {
                break;
            }

            for (int i = 0; i < xcnt; ++i) {
                matrix[row][col] = val++;
                col += xpos;
            }

            col -= xpos;
            xpos = -xpos;
            --xcnt;
            row += ypos;

            if (ycnt <= 0) {
                break;
            }

            for (int i = 0; i < ycnt; ++i) {
                matrix[row][col] = val++;
                row += ypos;
            }

            row -= ypos;
            ypos = -ypos;
            --ycnt;
            col += xpos;
        }

        return matrix;
    }
};
