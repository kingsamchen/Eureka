//
// Kingsley Chen
//

#include <iostream>
#include <string_view>

#include "src-loc/source_location.h"

void g(std::string_view msg) {
    std::cout << "in g():\n\t";
    auto s = base::msg_with_src_loc(msg);
    std::cout << s << "\n";
}

void f() {
    std::cout << "in f()\n";
    g("hello world");
}

int main()
{
    f();
    return 0;
}
