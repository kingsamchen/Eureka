#include <sstream>
#include <string>
#include <vector>

using namespace std;

// 核心思路
// 首先切分每个路径，只保留有效的component
// 然后逐一遍历每个component，跳过.，如果遇到..，还要把之前保存的component弹出来
class Solution {
public:
    string simplifyPath(string path) {
        auto tokens = Split(path);
        vector<string> cpts;
        for (const auto& tok : tokens) {
            if (tok == ".") {
                continue;
            }

            if (tok == "..") {
                if (!cpts.empty()) {
                    cpts.pop_back();
                }
                continue;
            }

            cpts.push_back(tok);
        }

        string simplified;
        for (auto&& cp : cpts) {
            simplified.append("/").append(std::move(cp));
        }

        return simplified.empty() ? "/" : simplified;
    }

    vector<string> Split(const std::string& s)
    {
        istringstream in(s);
        string token;
        vector<string> components;
        while (getline(in, token, '/')) {
            if (!token.empty()) {
                components.emplace_back(token);
            }
        }
        return components;
    }
};
