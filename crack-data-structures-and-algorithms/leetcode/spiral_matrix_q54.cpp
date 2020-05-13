#include <vector>

using namespace std;

// 核心思路
// 对于rows * cols的矩阵，进行如下设定
// xcnt: 水平方向（列）需要遍历的次数，初始值为 cols
// xpos: 遍历方向，1代表正向，-1代表反向；每个方向遍历结束后都需要取反
// ycnt: 垂直方向（行）需要遍历的次数，初始值为 rows - 1（注意这里-1）
// ypos: 和xpos类似
// xcnt 和 ycnt 在自己的方向遍历结束后需要自减
// (row, col)为矩阵坐标，初始为 (0,0)，受遍历操作影响
// 有了这几个定义之后就可以按照最普通的套路进行螺旋遍历了
// 首先水平方向遍历，完成后切换为垂直方向，然后再水平防线
// 直到 xcnt/ycnt 变为0 结束
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) {
            return {};
        }

        vector<int> result;

        int xcnt = matrix[0].size();
        int xpos = 1;
        int ycnt = matrix.size() - 1;
        int ypos = 1;
        int row = 0, col = 0;
        while (true) {
            if (xcnt <= 0) {
                break;
            }

            for (int i = 0; i < xcnt; ++i) {
                result.push_back(matrix[row][col]);
                col += xpos;
            }

            col -= xpos;
            --xcnt;
            xpos = -xpos;
            row += ypos;

            if (ycnt <= 0) {
                break;
            }

            for (int i = 0; i < ycnt; ++i) {
                result.push_back(matrix[row][col]);
                row += ypos;
            }

            row -= ypos;
            --ycnt;
            ypos = -ypos;
            col += xpos;
        }

        return result;
    }
};
