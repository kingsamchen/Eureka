//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef LUMPER_CLI_H_
#define LUMPER_CLI_H_

#include <map>
#include <string>
#include <variant>

#include "argparse/argparse.hpp"

namespace lumper {

class cli {
public:
    struct cmd_ps_t {};
    struct cmd_run_t {};

    static void init(int argc, const char* argv[]);

    static const cli& for_current_process();

    const std::string& command_name() const noexcept {
        return cur_cmd_parser_->first;
    }

    const argparse::ArgumentParser& command_parser() const noexcept {
        return cur_cmd_parser_->second.parser;
    }

    template<typename Visitor>
    void process_command(Visitor&& vis) const {
        std::visit(std::forward<Visitor>(vis), cur_cmd_parser_->second.cmd);
    }

private:
    cli(int argc, const char* argv[]);

private:
    using cmd_type = std::variant<cmd_ps_t, cmd_run_t>;

    struct cmd_parser {
        cmd_type cmd;
        argparse::ArgumentParser parser;
    };

    argparse::ArgumentParser prog_;
    std::map<std::string, cmd_parser> cmd_parser_table_;
    decltype(cmd_parser_table_)::iterator cur_cmd_parser_;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static inline const cli* current_{nullptr};
};

} // namespace lumper

#endif // LUMPER_CLI_H_
