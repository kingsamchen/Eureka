/*
 @ 0xCCCCCCCC
*/

#include "array_result.h"
#include "upper_case.h"
#include "string_sort.h"

template<typename T, size_t N>
constexpr auto GenerateArray() -> array_result<T, N>
{
    array_result<T, N> ary;

    for (size_t i = 0; i < N; ++i) {
        ary[i] = i + 1;
    }

    return ary;
}

void VerifyArrayResult()
{
    constexpr auto ary = GenerateArray<int, 3>();
    static_assert(ary[0] == 1, "Yep");
    static_assert(ary[1] == 2, "Yep");
    static_assert(ary[2] == 3, "Yep");
}

void VerifyCompileTimeUpperCase()
{
    constexpr auto str = MakeUpperCase("Hello, world");
    static_assert(str[0] == 'H', "Yek");
    static_assert(str[4] == 'O', "Yek");
    static_assert(str[5] == ',', "Yek");
}

void VerifyStringSort()
{
    static_assert(compile_time_string_less("abc", "bcd"), "Yak");

    constexpr const char* strings[] {
        "test",
        "hello",
        "jungle"
    };

    constexpr auto sorted = compile_time_str_sort(strings);

    static_assert(compile_time_string_equals(sorted[0], "hello"), "Yak");
    static_assert(compile_time_string_equals(sorted[2], "test"), "Yak");

    for (size_t i = 0; i < sorted.size(); ++i) {
        printf("%s\n", sorted[i]);
    }
}

int main()
{
    VerifyArrayResult();
    VerifyCompileTimeUpperCase();
    VerifyStringSort();

    return 0;
}
