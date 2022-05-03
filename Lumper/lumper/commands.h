//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef LUMPER_COMMANDS_H_
#define LUMPER_COMMANDS_H_

#include "lumper/cli.h"

#include <stdexcept>

namespace lumper {

class command_run_error : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

void process(cli::cmd_run_t);

void process(cli::cmd_ps_t);

} // namespace lumper

#endif // LUMPER_COMMANDS_H_
