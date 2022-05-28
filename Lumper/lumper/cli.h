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

#include "base/test_util.h"

namespace lumper {

class cli_parse_failure : public std::runtime_error {
public:
    cli_parse_failure(const std::string& what, const argparse::ArgumentParser* parser)
        : std::runtime_error(what),
          parser_(parser) {}

    const argparse::ArgumentParser& parser() const {
        return *parser_;
    }

private:
    const argparse::ArgumentParser* parser_;
};

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
    cli()
        : prog_("lumper") {}

    void parse(int argc, const char* argv[]);

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
    static inline cli* current_{nullptr};

    FRIEND_TEST_SUBCLASS(cli_test_stub);
};

} // namespace lumper

#endif // LUMPER_CLI_H_
