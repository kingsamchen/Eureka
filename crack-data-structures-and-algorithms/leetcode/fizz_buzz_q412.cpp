#include <string>
#include <vector>

using namespace std;

// 核心思路
// 非常直接的做法即可
class Solution {
public:
    vector<string> fizzBuzz(int n) {
        vector<string> result;
        for (auto i = 1; i <= n; ++i) {
            if (i % 15 == 0) {
                result.push_back("FizzBuzz");
            } else if (i % 3 == 0) {
                result.push_back("Fizz");
            } else if (i % 5 == 0) {
                result.push_back("Buzz");
            } else {
                result.push_back(to_string(i));
            }
        }
        return result;
    }
};
