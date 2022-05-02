//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#pragma once

#ifndef LUMPER_COMMANDS_H_
#define LUMPER_COMMANDS_H_

#include "lumper/cli.h"

namespace lumper {

void process(cli::cmd_run_t);

void process(cli::cmd_ps_t);

} // namespace lumper

#endif // LUMPER_COMMANDS_H_
