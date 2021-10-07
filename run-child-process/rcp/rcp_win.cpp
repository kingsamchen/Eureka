#include "rcp/rcp.h"

#include <Windows.h>

namespace rcp {

child_process run_child_process(const std::string& pathname, const std::vector<std::string>& args) {
    auto std_in = make_pipe();
    auto std_out = make_pipe();

    ::SetHandleInformation(std_in.write_end().get(), HANDLE_FLAG_INHERIT, 0);
    ::SetHandleInformation(std_out.read_end().get(), HANDLE_FLAG_INHERIT, 0);

    STARTUPINFO startup_info{};
    startup_info.cb = sizeof(startup_info);
    startup_info.dwFlags = STARTF_USESTDHANDLES;
    startup_info.hStdInput = std_in.read_end().get();
    startup_info.hStdOutput = std_out.write_end().get();

    PROCESS_INFORMATION proc_info{};

    std::string cmdline(pathname);
    for (const auto& arg : args) {
        cmdline.append(1, ' ').append(arg);
    }

    // including trailing \0
    std::vector<wchar_t> cmdline_buf(cmdline.size() + 1);
    size_t cvt_cnt = 0;
    if (mbstowcs_s(&cvt_cnt, cmdline_buf.data(), cmdline_buf.size(), cmdline.c_str(),
                   cmdline.size()) != 0) {
        throw std::system_error(std::error_code(errno, std::generic_category()),
                                "mbstowcs_s failure");
    }

    if (!::CreateProcessW(nullptr,
                          cmdline_buf.data(),
                          nullptr,
                          nullptr,
                          TRUE,
                          0,
                          nullptr,
                          nullptr,
                          &startup_info,
                          &proc_info)) {
        throw std::system_error(std::error_code(::GetLastError(), std::system_category()),
                                "CreateProcessW() failure");
    }

    ::CloseHandle(proc_info.hThread);

    return {wrap_unique_win_handle(proc_info.hProcess), std::move(std_in), std::move(std_out)};
}

void child_process::wait() const noexcept {
    ::WaitForSingleObject(cpid_.get(), INFINITE);
}

} // namespace rcp
