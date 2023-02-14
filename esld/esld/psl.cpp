//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "esld/psl.h"

#include <algorithm>

#include "fmt/format.h"

#include "esl/file_util.h"
#include "esl/strings.h"

namespace strings = esl::strings;

using namespace std::string_view_literals;

namespace psl {
namespace {

struct skip_comment {
    bool operator()(std::string_view str) const noexcept {
        return !strings::starts_with(str, "//");
    }
};

enum class rule_type {
    normal,
    wildcard,
    exception
};

} // namespace

std::string from_file(const std::string& path) {
    std::string data;
    esl::read_file_to_string(path, data);
    return from_data(data);
}

std::string from_data(std::string_view data) {
    auto rules = strings::split(data, strings::by_any_char{"\r\n"}, [](auto sv) {
                     return strings::skip_empty{}(sv) && skip_comment{}(sv);
                 }).to<std::vector<std::string_view>>();
    auto rtl_cmp = [](std::string_view lhs, std::string_view rhs) -> bool {
        auto delim = strings::by_char{'.'};
        auto l_parts = strings::split(lhs, delim).to<std::vector<std::string_view>>();
        assert(!l_parts.empty());
        auto r_parts = strings::split(rhs, delim).to<std::vector<std::string_view>>();
        assert(!r_parts.empty());

        auto l_rit = l_parts.rbegin();
        auto r_rit = r_parts.rbegin();
        for (; l_rit != l_parts.rend() && r_rit != r_parts.rend(); ++l_rit, ++r_rit) {
            if (std::lexicographical_compare(l_rit->cbegin(), l_rit->cend(),
                                             r_rit->cbegin(), r_rit->cend())) {
                return true;
            }

            if (std::lexicographical_compare(r_rit->cbegin(), r_rit->cend(),
                                             l_rit->cbegin(), l_rit->cend())) {
                return false;
            }
        }

        return l_rit == l_parts.rend();
    };
    std::sort(rules.begin(), rules.end(), rtl_cmp);
    auto buf = strings::join(rules, rule_delim);
    return buf;
}

std::string_view match_tld(std::string_view rule_data, const std::string& domain) {
    if (domain.empty()) {
        throw std::invalid_argument("domain is empty");
    }

    std::string_view tld;
    std::size_t start = 0;

    while (true) {
        assert(domain.size() >= tld.size());
        if (domain.size() == tld.size()) {
            return tld;
        }
        auto dot_chk_pos = domain.size() - 1 - tld.size();
        if (domain[dot_chk_pos] == '.') {
            --dot_chk_pos;
        }

        auto dot_pos = domain.rfind('.', dot_chk_pos);
        std::string_view test_label{&domain[dot_pos + 1], domain.size() - dot_pos};
        fmt::print(stderr, "---\ntld={}\ntest_label={}\n", tld, test_label);

        while (true) {
            auto pos = rule_data.find(test_label, start);
            if (pos == std::string_view::npos) {
                return tld;
            }

            auto rule_begin = rule_data.rfind(rule_delim, pos) + 1;
            std::size_t rule_len = pos + test_label.size() - 1 - rule_begin;
            auto rule = rule_data.substr(rule_begin, rule_len);

            auto hit_type = rule_type::normal;
            if (strings::starts_with(rule, "!")) {
                hit_type = rule_type::exception;
                rule = strings::trim_left(rule, "!");
            } else if (strings::starts_with(rule, "*.")) {
                hit_type = rule_type::wildcard;
                rule = strings::trim_left(rule, "*.");
            }

            fmt::print(stderr, "hit-rule={}\trule-type={}\n", rule, static_cast<int>(hit_type));

            start = pos + test_label.size();

            // Hit rule is not a match.
            if (!strings::ends_with(domain, rule)) {
                fmt::print(stderr, "not a match, continue searching\n");
                continue;
            }

            // Exception rule is in highest priority.
            if (hit_type == rule_type::exception) {
                return rule.substr(rule.find('.') + 1);
            }

            if (hit_type == rule_type::wildcard) {
                if (domain.size() == rule.size()) {
                    return rule;
                }
                auto chk_pos = domain.size() - 1 - rule.size();
                if (domain[chk_pos] == '.') {
                    assert(chk_pos > 0);
                    --chk_pos;
                }
                tld = std::string_view{&domain[domain.rfind('.', chk_pos) + 1]};
            } else {
                tld = rule;
            }

            fmt::print(stderr, "update tld={}\n", tld);

            break;
        }
    }
}

} // namespace psl
