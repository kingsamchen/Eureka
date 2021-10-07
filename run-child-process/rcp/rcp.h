#pragma once

#ifndef RCP_RCP_H_
#define RCP_RCP_H_

#include <string>
#include <vector>

#include "rcp/pipe.h"

namespace rcp {

// Omit stderr
struct child_process {
#if defined(OS_WIN)
    using process_handle_t = unique_win_handle;
#else
    using process_handle_t = pid_t;
#endif
    process_handle_t cpid_;
    pipe stdin_;
    pipe stdout_;

    pipe::native_handle_t std_in() const noexcept {
        return stdin_.write_end().get();
    }

    pipe::native_handle_t std_out() const noexcept {
        return stdout_.read_end().get();
    }

    void shutdown() noexcept {
        // shutdown write to send EOF.
        stdin_.reset();
    }

    void wait() const noexcept;
};

child_process run_child_process(const std::string& pathname,
                                const std::vector<std::string>& args = {});

} // namespace rcp

#endif // RCP_RCP_H_
