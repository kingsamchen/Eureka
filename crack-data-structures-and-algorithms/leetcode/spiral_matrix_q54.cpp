#include <vector>

using namespace std;

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
