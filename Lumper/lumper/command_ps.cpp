//
// Kingsley Chen <kingsamchen at gmail dot com>
//

#include "lumper/commands.h"

#include "fmt/core.h"

namespace lumper {

void process(cli::cmd_ps_t) {
    fmt::print("lumper ps!");
}

} // namespace lumper
