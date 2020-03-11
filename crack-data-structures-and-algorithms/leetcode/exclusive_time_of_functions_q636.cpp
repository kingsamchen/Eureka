#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// 核心思路
// 函数调用形成栈帧，所以我们需要用一个栈来维护函数调用的运行时间
// 栈 entry 结构为 <function_id, latest_ts, dur>
// 如果一个函数退出了，那么它的开始一定已经记录在栈顶，利用这点就可以计算函数运行时间
// 上一个函数出栈之后需要更新当前栈顶的 latest_ts
// 另外要注意两点：
// 1) 函数从t1开始t2结束，他的运行时间是[t1, t2]，时长应该是 t2 - t1 + 1
// 更新当前栈顶的 latest_ts 时应为 ts + 1，因为从ts下一个时刻才继续开始运行
// 2) 函数有可能递归，但是递归调用会创建新的栈帧，所以上面逻辑不需要变化，但是最终输出结果
// 需要累加
class Solution {
public:
    vector<int> exclusiveTime(int n, vector<string>& logs) {
        vector<int> result(n, 0);
        vector<entry> run_stack;
        for (const auto& rec : logs) {
            int fid, ts;
            string behavior;
            tie(fid, behavior, ts) = extract_log_info(rec);
            if (behavior == "start") {
                if (run_stack.empty()) {
                    run_stack.emplace_back(fid, ts);
                    continue;
                }

                auto delta = ts - run_stack.back().latest_ts;
                run_stack.back().dur += delta;
                run_stack.emplace_back(fid, ts);
            } else {
                auto delta = ts + 1 - run_stack.back().latest_ts;
                run_stack.back().dur += delta;
                result[run_stack.back().fid] += run_stack.back().dur;
                run_stack.pop_back();

                if (!run_stack.empty()) {
                    run_stack.back().latest_ts = ts + 1;
                }
            }
        }

        return result;
    }

    tuple<int, string, int> extract_log_info(const string& log) {
        istringstream in(log);
        string field;
        vector<string> fields;
        while (getline(in, field, ':')) {
            fields.push_back(field);
        }
        return {stoi(fields[0]), fields[1], stoi(fields[2])};
    }

    struct entry {
        int fid;
        int latest_ts;
        int dur;

        entry(int fid, int ts)
            : fid(fid), latest_ts(ts), dur(0)
        {}
    };
};
