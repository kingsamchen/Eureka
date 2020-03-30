#include <algorithm>
#include <vector>

using namespace std;

// 核心思路
// 将会议的开始时间/结束时间分别保存到st/et两个数组里，并且分别排序
// 对于 st 的每个开始时间，要么复用一个空的会议室，要么开一个新的会议室
// 而 et 数组维护了按时间排序的下一个可用会议室的开始时间，所以
// 1) 如果某个 stime < et[pos] 则说明目前没有可用会议室了，因为最接近的可用会议室要等到 et[pos]，需要新开会议室
// 2) 反之如果 stime >= et[pos] 则说明 et[pos] 时间开始可用的会议室可以被 stime 开始的会议复用，我们需要自增
//    pos 去检查下一个会议室的可用开始时间
class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& intervals) {
        vector<int> st;
        vector<int> et;
        for (const auto& pair : intervals) {
            st.push_back(pair[0]);
            et.push_back(pair[1]);
        }

        sort(st.begin(), st.end());
        sort(et.begin(), et.end());

        int room = 0;
        int epos = 0;
        for (int stime : st) {
            if (stime < et[epos]) {
                ++room;
            } else {
                ++epos;
            }
        }

        return room;
    }
};
