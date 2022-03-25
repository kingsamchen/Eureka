//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "lumper/cli.h"

#include "fmt/ranges.h"

void process(lumper::cli::cmd_run_t) {
    const auto& parser = lumper::cli::for_current_process().command_parser();
    auto maybe_args = parser.present<std::vector<std::string>>("ARGS");
    fmt::print("cmd={}\nargs={}\n--it={}",
               parser.get<std::string>("CMD"),
               maybe_args.has_value() ? *maybe_args : std::vector<std::string>{},
               parser.get<bool>("--it"));
}

void process(lumper::cli::cmd_ps_t) {
    fmt::print("lumper ps!");
}

int main(int argc, const char* argv[]) {
    lumper::cli::init(argc, argv);
    lumper::cli::for_current_process().process_command([](auto cmd) {
        process(cmd);
    });
    return 0;
}
