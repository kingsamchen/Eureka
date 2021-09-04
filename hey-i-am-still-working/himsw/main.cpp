// Copyright (c) 2021 Kingsley Chen <kingsamchen at gmail dot com>
// This file is subject to the terms of license that can be found
// in the LICENSE file.

#include <memory>
#include <string>
#include <thread>

#include <Windows.h>

#include "nana/gui.hpp"
#include "nana/gui/timer.hpp"
#include "nana/gui/widgets/label.hpp"
#include "spdlog/fmt/fmt.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

#include "himsw/labor_monitor.h"
#include "himsw/win_last_error.h"

namespace {

template<typename T>
void ignore_result(T&&) {}

class main_window : public nana::form {
    struct passkey {
        explicit passkey() = default;
    };

public:
    main_window(const main_window&) = delete;

    main_window(main_window&&) = delete;

    main_window& operator=(const main_window&) = delete;

    main_window& operator=(main_window&&) = delete;

    static std::shared_ptr<main_window> make() {
        auto main = std::make_shared<main_window>(passkey{});
        main->show();
        return main;
    }

    explicit main_window(passkey)
        : nana::form(),
          lbl_info_(*this, true) {
        this->caption("Hey I AM Still Working");

        lbl_info_.size(this->size());
        lbl_info_.caption("monitoring...");

        timer_.elapse([] {
            himsw::labor_monitor::instance().tick();
        });
        timer_.interval(std::chrono::seconds(30));
        timer_.start();
    }

    void update_info(const std::string& msg) {
        lbl_info_.caption(msg);
    }

private:
    nana::label lbl_info_;
    nana::timer timer_;
};

} // namespace

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    constexpr int critical_failure = 1;

    try {
        spdlog::set_default_logger(spdlog::basic_logger_mt("main_logger", "himsw.log", true));
        himsw::labor_monitor::instance().prepare();
    } catch (const spdlog::spdlog_ex& ex) {
        auto reason = fmt::format("Failed to initialize logging component; ex={}", ex.what());
        nana::msgbox mb(reason);
        ignore_result(mb.show());
        return critical_failure;
    } catch (const himsw::win_last_error& ex) {
        const std::string cause = "Failed to prepare the labor monitor";
        spdlog::error("{}; ex={} ec={}", cause, ex.what(), ex.error_code());
        nana::msgbox mb(cause);
        ignore_result(mb.show());
        return critical_failure;
    }

    ::SetProcessDPIAware();

    auto window = main_window::make();
    himsw::labor_monitor::instance().set_info_update_handler(
        [ptr = std::weak_ptr(window)](const std::string& msg) {
            if (auto wnd = ptr.lock(); wnd) {
                wnd->update_info(msg);
            }
        });

    nana::exec();

    return 0;
}
