//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "doctest/doctest.h"

#include "base/subprocess.h"

#include "fmt/core.h"

TEST_CASE("Demo") {
    auto opts = base::subprocess::options().set_stdout(base::subprocess::use_pipe);
    base::subprocess proc({"/bin/ls", "/home/kingsleychen"}, opts);
    REQUIRE(proc.stdout_pipe() != -1);
    constexpr size_t buf_size = 256;
    char buf[buf_size]{};
    auto bytes_read = ::read(proc.stdout_pipe(), buf, buf_size);
    REQUIRE(bytes_read > 0);
    fmt::print(stderr, "{}", std::string(buf, bytes_read));
    proc.wait();
}
