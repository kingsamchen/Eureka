#include <cstdio>
#include <string>

int main()
{
    std::string ss[] {
        "hello",
        "world",
        "this is a test"
    };

    for (const auto& s : ss) {
        printf("%s\n", s.c_str());
    }

    return 0;
}
