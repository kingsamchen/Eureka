
#include <conio.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <set>

#include "kbase/command_line.h"
#include "kbase/file_util.h"
#include "kbase/path.h"
#include "kbase/tokenizer.h"
#include "kbase/sys_string_encoding_conversions.h"

std::random_device rd;

std::vector<std::string> RandomSelect(const std::set<std::string>& dataset,
                                      int combination_count,
                                      int group_count)
{
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<size_t> dist(0, dataset.size() - 1);
    std::vector<std::string> candidates;
    for (int i = 0; i < group_count; ++i) {
        std::string token;
        for (int j = 0; j < combination_count; ++j) {
            auto it = std::next(dataset.begin(), dist(engine));
            token += *it;
        }

        candidates.push_back(token);
    }

    return candidates;
}

int main()
{
    //std::cin.get();
    auto& cmdline = kbase::CommandLine::ForCurrentProcess();
    auto params = cmdline.GetParameters();
    if (params.empty()) {
        std::cout << "No data source found!\n";
        return 0;
    }

    std::wstring data_type;
    if (!cmdline.GetSwitchValue(L"type", &data_type)) {
        std::cout << "No source type category found!\n";
        return 0;
    }

    std::cout << "Reading data...\n";

    std::string content = kbase::ReadFileToString(kbase::Path(params[0]));
    kbase::Tokenizer tokenizer(content, "\r\n\t ");
    std::set<std::string> tokens;
    for (auto&& token : tokenizer) {
        auto&& str = token.ToString();
        if (!str.empty()) {
            tokens.insert(str);
        }
    }

    int combination_count = data_type == L"words" ? 4 : 2;
    int group_count = 10;

    std::cout << "Done.\nPress any (but x) to review random phases\n";

    char ch;
    while ((ch = _getch()) != 'x') {
        auto results = RandomSelect(tokens, combination_count, group_count);
        std::copy(results.begin(), results.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
        std::cout << "---\n";
    }

    std::cout << "Goodbye.";

    return 0;
}

