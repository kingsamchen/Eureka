#pragma once

#ifndef RCP_RCP_H_
#define RCP_RCP_H_

#include <string>
#include <vector>

#include "rcp/pipe.h"

namespace rcp {

// Omit stderr
struct child_process {
    pid_t cpid;
    pipe std_in;
    pipe std_out;

    pipe::native_handle_t stdin() const noexcept {
        return std_in.write_end().get();
    }

    pipe::native_handle_t stdout() const noexcept {
        return std_out.read_end().get();
    }

    void shutdown() {
        // shutdown write to send EOF.
        std_in.reset();
    }

    void join();
};

child_process run_child_process(const std::string& pathname,
                                const std::vector<std::string>& args = {});

} // namespace rcp

#endif // RCP_RCP_H_
