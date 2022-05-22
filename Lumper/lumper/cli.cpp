//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "lumper/cli.h"

#include <stdexcept>
#include <string_view>

#include "esl/strings.h"
#include "fmt/printf.h"
#include "fmt/ranges.h"

namespace lumper {
namespace {

constexpr char k_prog_cmd[] = "COMMAND";
constexpr char k_cmd_run[] = "run";
constexpr char k_cmd_ps[] = "ps";

class cli_parse_failure : public std::runtime_error {
public:
    cli_parse_failure()
        : runtime_error("cli parse failure") {}
};

} // namespace

// static
void cli::init(int argc, const char* argv[]) {
    try {
        static cli instance(argc, argv);
        current_ = &instance;
    } catch (const cli_parse_failure&) {
        std::exit(1);
    }
}

// static
const cli& cli::for_current_process() {
    if (!current_) {
        fmt::print(stderr, "cli is not inited!");
        std::abort();
    }
    return *current_;
}

cli::cli(int argc, const char* argv[])
    : prog_("lumper") {
    argparse::ArgumentParser parser_run("lumper run");
    parser_run.add_argument("--it")
            .help("enable tty")
            .nargs(0)
            .default_value(false)
            .implicit_value(true);
    parser_run.add_argument("-m", "--memory")
            .help("enable memory limit");
    parser_run.add_argument("CMD")
            .help("executable and its arguments (optional)")
            .remaining();
    cmd_parser_table_.emplace(k_cmd_run, cmd_parser{cmd_run_t{}, std::move(parser_run)});

    argparse::ArgumentParser parser_ps("lumper ps");
    parser_ps.add_argument("-a", "--all")
            .help("Show all containers")
            .nargs(0)
            .default_value(false)
            .implicit_value(true);
    cmd_parser_table_.emplace(k_cmd_ps, cmd_parser{cmd_ps_t{}, std::move(parser_ps)});

    prog_.add_argument(k_prog_cmd)
            .remaining()
            .help("Avaliable commands: \n  " +
                  esl::strings::join(cmd_parser_table_,
                                     "\n  ",
                                     [](const auto& e, std::string& ap) { ap.append(e.first); }));

    std::vector<std::string> prog_args;
    argparse::ArgumentParser* cur_parser{&prog_};
    try {
        prog_.parse_args(argc, argv);
        prog_args = prog_.get<std::vector<std::string>>(k_prog_cmd);
        const auto& cmd = prog_args[0];
        cur_cmd_parser_ = cmd_parser_table_.find(cmd);
        if (cur_cmd_parser_ == cmd_parser_table_.end()) {
            throw std::runtime_error("Unknown command: " + cmd);
        }

        cur_parser = &cur_cmd_parser_->second.parser;
        cur_parser->parse_args(prog_args);
    } catch (const std::logic_error& ex) {
        fmt::print(stderr, "{}\n\n{}", ex.what(), cur_parser->help().str());
        throw cli_parse_failure{};
    } catch (const std::runtime_error& ex) {
        fmt::print(stderr, "{}\n\n{}", ex.what(), cur_parser->help().str());
        throw cli_parse_failure{};
    }
}

} // namespace lumper
