//
// Kingsley Chen
//

#include <vector>

#include "fmt/format.h"
#include "esl/strings.h"
#include "spdlog/spdlog.h"
#include "uuidxx/uuidxx.h"

void test_esl() {
    fmt::println("[test_esl]");
    fmt::println(esl::strings::join(std::vector{"hello", "world", "esl", "test"}, " "));
    fmt::println("\n");
}

void test_uuidxx() {
    fmt::println("[test_uuidxx]");
    fmt::println(uuidxx::make_v4().to_string());
    fmt::println("\n");
}

void test_spdlog() {
    fmt::println("[test_spdlog]");
    SPDLOG_INFO("this is from {}", "spdlog");
    fmt::println("\n");
}

int main() {
    test_esl();
    test_uuidxx();
    test_spdlog();
    return 0;
}
