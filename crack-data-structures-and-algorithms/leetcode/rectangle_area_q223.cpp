// 核心思路
// 如果两个矩形没有交集，则最后的面积和是 sum1 + sum2
// 否则，sum1 + sum2 - 重叠区域
// 重叠区域判断要考虑有可能：R1 相对于 R2 左侧；R2 相对于 R1 左侧
// 画图形有助于分辨
class Solution {
public:
    int computeArea(int A, int B, int C, int D, int E, int F, int G, int H) {
        int sum1 = (C - A) * (D - B), sum2 = (H - F) * (G - E);

        if (E >= C || F >= D || B >= H || A >= G) {
            return sum1 + sum2;
        }

        return sum1 - ((min(G, C) - max(A, E)) * (min(D, H) - max(B, F))) + sum2;
    }
};
