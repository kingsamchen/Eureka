// Copyright (c) 2021 Kingsley Chen <kingsamchen at gmail dot com>
// This file is subject to the terms of license that can be found
// in the LICENSE file.

#include <deque>
#include <memory>
#include <string>
#include <string_view>
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

template<typename Iter>
std::string join_string(Iter begin, Iter end, std::string_view sep) {
    if (begin == end) {
        return std::string{};
    }

    std::string str(*begin);
    for (auto it = begin + 1; it != end; ++it) {
        str.append(sep.data(), sep.length()).append(*it);
    }

    return str;
}

std::string now_in_date() {
    auto tp = std::time(nullptr);
    struct tm tm;
    localtime_s(&tm, &tp);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S ");
    return oss.str();
}

nana::size calc_initial_window_size() {
    constexpr auto k_ratio = 0.618;
    auto monitor = nana::screen::primary_monitor_size();
    auto init_width = static_cast<nana::size::value_type>(monitor.width * k_ratio + 0.5);
    auto init_height = static_cast<nana::size::value_type>(monitor.height * k_ratio + 0.5);
    return {init_width / 2, init_height / 2};
}

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
        auto main = std::make_shared<main_window>(passkey{}, calc_initial_window_size());
        main->show();
        return main;
    }

    explicit main_window(passkey, nana::size size)
        : nana::form(nana::API::make_center(size.width, size.height)) {
        this->caption("Hey I AM Still Working");

        lbl_info_.size(this->size());
        lbl_info_.caption("monitoring...");

        place_.div("<msg>");
        place_["msg"] << lbl_info_;
        place_.collocate();

        timer_.elapse([] {
            himsw::labor_monitor::instance().tick();
        });
        timer_.interval(std::chrono::seconds(10));
        timer_.start();
    }

    void update_info(const std::string& msg) {
        msgs_.push_front(now_in_date() + msg);
        if (msgs_.size() > k_max_kept_msgs) {
            msgs_.pop_back();
        }

        lbl_info_.caption(join_string(msgs_.begin(), msgs_.end(), "\r\n"));
    }

private:
    nana::label lbl_info_{*this, true};
    nana::place place_{*this};
    nana::timer timer_;
    std::deque<std::string> msgs_;
    static constexpr size_t k_max_kept_msgs{10};
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
