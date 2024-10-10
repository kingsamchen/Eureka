//
// Kingsley Chen
//

#include <cassert>
#include <iostream>
#include <ranges>
#include <string_view>
#include <vector>

#include "fmt/format.h"
#include "fmt/printf.h"
#include "fmt/ranges.h"

#include "ranges/chain_view.h"

int fib(std::vector<int>& memo, size_t n) {
    if (n <= memo.size()) {
        return memo[n - 1];
    }

    auto val = fib(memo, n - 1) + fib(memo, n - 2);
    memo.push_back(val);
    return val;
}

void print_first_n_dividable_fib(int n, int k) {
    if (n <= 0) {
        return;
    }

    std::vector<int> memo{0, 1};
    auto r = std::views::iota(1U) |
             std::views::transform([&memo](size_t i) { return fib(memo, i); }) |
             std::views::filter([k](int val) { return val % k == 0; }) |
             std::views::take(n);
    for (auto v : r) {
        std::cout << v << " ";
    }

    std::cout << "\n";
}

void trim_spaces(std::string_view s) {
    auto trimed = s | std::views::drop_while(::isspace) |
                  std::views::reverse |
                  std::views::drop_while(::isspace) |
                  std::views::reverse;

    // Workaround: views are usually lazy and thus no contiguous guarantee.
    auto len = std::distance(trimed.begin(), trimed.end());
    auto sv = std::string_view(&*trimed.begin(), static_cast<size_t>(len));
    std::cout << s << " -> " << sv << "\n";
}

void cyclic_shift_left_inplace(std::vector<int>& v, std::size_t k) {
    auto t = static_cast<std::decay_t<decltype(v)>::difference_type>(k % v.size());
    std::ranges::rotate(v, std::next(v.begin(), t));
    fmt::println("{}", v);
}

void chain_up_views() {
    std::vector v1{1, 3, 5, 7};
    std::vector v2{2, 4, 6};

    chain_view cv(std::views::all(v1), std::views::all(v2));
    fmt::println("{}", cv);
}

int main() {
    // print_first_n_dividable_fib(5, 3);
    // trim_spaces("\n\rhello world\t\r\n");
    // std::vector<int> v{1,2,3,4,5};
    // cyclic_shift_left_inplace(v, 3U);
    chain_up_views();
    return 0;
}
