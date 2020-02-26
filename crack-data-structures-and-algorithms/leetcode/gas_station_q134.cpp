#include <vector>

using namespace std;

// 核心思路
// 使用两个累计变量跟踪油量和旅途消耗差值
// total: 全局的 gas[i] - cost[i]；要能跑完一圈，总量肯定要至少不小于0
// accum: 从起点开始的累计 gas[i] - cost[i]；如果对某个station i，导致accum < 0
// 那么说明从[start, i] 之间的加油站都不能作为起点；这个时候只能尝试i+1为起点的case
class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int total = 0;
        int accum = 0, start = 0;
        for (auto i = start; i < gas.size(); ++i) {
            auto delta = gas[i] - cost[i];
            total += delta;
            accum += delta;
            if (accum < 0) {
                accum = 0;
                start = i + 1;
            }
        }

        return total < 0 ? -1 : start;
    }
};
