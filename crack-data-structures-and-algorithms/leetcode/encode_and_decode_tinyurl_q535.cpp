#include <random>
#include <string>
#include <unordered_map>

using namespace std;

// 核心思路
// 对于encode，随机生成6个[0, 64)的整数，对应字母表的某个字符，作为随机码
// 将 <code, longurl> 存入 map，并且将随机码作为short-url的一部分返回
// 解吗就是拿到随机码去table寻找
// 这里假设 1/2^64 概率足够小，其实可以再出现重复的时候再生成一次；这个收敛应该很快
class Solution {
public:
    unordered_map<string, string> mapping_;
    const char* const kPrefix = "http://tinyurl.com/";
    // len = 64
    const char* const kTable = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-";

    default_random_engine engine_;
    uniform_int_distribution<> dist;

    Solution()
        : engine_(random_device{}()), dist(0, 63)
    {}

    // Encodes a URL to a shortened URL.
    string encode(string longUrl) {
        string code;
        for (auto i = 0; i < 6; ++i) {
            code.push_back(kTable[dist(engine_)]);
        }
        // Suppose 1 / 2^64 is enough for uniqueness.
        mapping_[code] = longUrl;
        return kPrefix + code;
    }

    // Decodes a shortened URL to its original URL.
    string decode(string shortUrl) {
        auto pos = shortUrl.find_last_of('/');
        auto code = shortUrl.substr(pos + 1);
        return mapping_[code];
    }
};
