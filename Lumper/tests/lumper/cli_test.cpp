//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "doctest/doctest.h"

#include "lumper/cli.h"

#include "tests/stringification.h"

namespace lumper {

class cli_test_stub : public cli {
public:
    using cli::parse;
};

} // namespace lumper

namespace {

using lumper::cli_test_stub;

template<typename T, int N>
int ssize(T (&)[N]) {
    return N;
}

template<typename T>
int ssize(const std::vector<T>& v) {
    return static_cast<int>(v.size());
}

TEST_SUITE_BEGIN("cli");

TEST_CASE("command is mandatory") {
    SUBCASE("throws when command missing") {
        const char* args[] = {"./lumper"};
        cli_test_stub cli;
        CHECK_THROWS(cli.parse(ssize(args), args));
    }

    SUBCASE("throws when unknown command") {
        const char* args[] = {"./lumper", "test"};
        cli_test_stub cli;
        CHECK_THROWS(cli.parse(ssize(args), args));
    }
}

TEST_CASE("command run") {
    std::vector<const char*> args{"./lumper", "run"};
    CAPTURE(args);

    SUBCASE("empty when no cmd provided") {
        cli_test_stub cli;
        cli.parse(ssize(args), args.data());
        CHECK_THROWS(cli.command_parser().get<std::vector<std::string>>("CMD"));
    }

    SUBCASE("run executable only") {
        args.push_back("some_cmd");
        cli_test_stub cli;
        cli.parse(ssize(args), args.data());
        auto cmd = cli.command_parser().get<std::vector<std::string>>("CMD");
        REQUIRE_EQ(cmd.size(), 1);
        CHECK_EQ(cmd[0], "some_cmd");
    }

    SUBCASE("run executable with args") {
        auto exec = {"some_cmd", "arg1", "--flag1", "args2", "--flag2"};
        args.insert(args.end(), exec);
        cli_test_stub cli;
        cli.parse(ssize(args), args.data());
        auto cmd = cli.command_parser().get<std::vector<std::string>>("CMD");
        REQUIRE_EQ(cmd.size(), 5);
        CHECK_EQ(cmd, std::vector<std::string>(exec.begin(), exec.end()));
    }

    SUBCASE("support enable-tty flag") {
        SUBCASE("false when no specified") {
            args.push_back("some_cmd");
            cli_test_stub cli;
            cli.parse(ssize(args), args.data());
            CHECK_FALSE(cli.command_parser().get<bool>("--it"));
        }

        SUBCASE("with being explicitly given") {
            args.push_back("--it");
            args.push_back("some_cmd");
            cli_test_stub cli;
            cli.parse(ssize(args), args.data());
            CHECK(cli.command_parser().get<bool>("--it"));
        }
    }

    SUBCASE("support memory-limit flag") {
        SUBCASE("specify memory limits") {
            args.insert(args.end(), {"-m", "10m", "some_cmd"});
            cli_test_stub cli;
            cli.parse(ssize(args), args.data());
            CHECK_EQ(cli.command_parser().get<std::string>("--memory"), "10m");
        }

        SUBCASE("not present when not specified") {
            args.push_back("some_cmd");
            cli_test_stub cli;
            cli.parse(ssize(args), args.data());
            auto m = cli.command_parser().present("--memory");
            CHECK_FALSE(m.has_value());
        }
    }
}

TEST_CASE("command ps") {
    std::vector<const char*> args{"./lumper", "ps"};

    SUBCASE("implicit --all argument") {
        cli_test_stub cli;
        cli.parse(ssize(args), args.data());
        CHECK_EQ(cli.command_name(), "ps");
        CHECK_FALSE(cli.command_parser().get<bool>("--all"));
    }

    SUBCASE("exlicitly specify --all") {
        args.push_back("--all");
        cli_test_stub cli;
        cli.parse(ssize(args), args.data());
        CHECK_EQ(cli.command_name(), "ps");
        CHECK(cli.command_parser().get<bool>("--all"));
    }

    SUBCASE("short for -a") {
        args.push_back("-a");
        cli_test_stub cli;
        cli.parse(ssize(args), args.data());
        CHECK_EQ(cli.command_name(), "ps");
        CHECK(cli.command_parser().get<bool>("--all"));
    }
}

TEST_SUITE_END();

} // namespace
