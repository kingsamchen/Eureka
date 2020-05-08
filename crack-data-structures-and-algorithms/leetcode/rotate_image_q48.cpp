#include <vector>

using namespace std;

// 核心思路
// 1) 首先将矩阵按行逆序
//    12       34
//    34       12
// 2) 然后按照(0,0) -> (n,n) 的斜对角线做一个轴对称将两边的值交换
//    34        31
//    12        42
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        reverse(matrix.begin(), matrix.end());
        for (int row = 0; row < matrix.size(); ++row) {
            for (int col = row + 1; col < matrix[0].size(); ++col) {
                swap(matrix[row][col], matrix[col][row]);
            }
        }
    }
};
