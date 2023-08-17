// Copyright (c) 2021 Kingsley Chen <kingsamchen at gmail dot com>
// This file is subject to the terms of license that can be found
// in the LICENSE file.

#include <algorithm>
#include <deque>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include <Windows.h>
#include <windowsx.h>

#include "spdlog/fmt/fmt.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

#include "himsw/labor_monitor.h"
#include "himsw/resource.h"
#include "himsw/tray.h"
#include "himsw/win_last_error.h"

namespace {

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
    tm tm{};
    localtime_s(&tm, &tp);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S ");
    return oss.str();
}

class dialog_window;

class dialog_window_manager {
public:
    ~dialog_window_manager() = default;

    dialog_window_manager(const dialog_window_manager&) = delete;

    dialog_window_manager(dialog_window_manager&&) = delete;

    dialog_window_manager& operator=(const dialog_window_manager&) = delete;

    dialog_window_manager& operator=(dialog_window_manager&&) = delete;

    static dialog_window_manager& instance() {
        static dialog_window_manager instance;
        return instance;
    }

    void enroll(HWND hwnd, dialog_window* ptr) {
        dlg_table_[hwnd] = ptr;
    }

    void unroll(HWND hwnd) {
        dlg_table_.erase(hwnd);
        if (dlg_table_.empty()) {
            PostQuitMessage(0);
        }
    }

    bool any_dialog_message_processed(MSG* msg) {
        return std::any_of(dlg_table_.begin(), dlg_table_.end(), [msg](auto& entry) {
            return ::IsDialogMessageW(entry.first, msg) == TRUE;
        });
    }

    static INT_PTR dialog_proc(HWND dlg, UINT msg, WPARAM wparam, LPARAM lparam);

private:
    dialog_window_manager() = default;

private:
    std::unordered_map<HWND, dialog_window*> dlg_table_;
};

class dialog_window {
    struct passkey {
        explicit passkey() = default;
    };

public:
    ~dialog_window() = default;

    dialog_window(const dialog_window&) = delete;

    dialog_window(dialog_window&&) = delete;

    dialog_window& operator=(const dialog_window&) = delete;

    dialog_window& operator=(dialog_window&&) = delete;

    static std::shared_ptr<dialog_window> make() {
        auto window = std::make_shared<dialog_window>(passkey{});
        dialog_window_manager::instance().enroll(window->dlg_, window.get());
        window->show();

        auto chk_hwnd = ::GetDlgItem(window->dlg_, IDC_CHKSTATUS);
        Button_SetCheck(chk_hwnd, BST_CHECKED);
        ::SendMessageW(window->dlg_, WM_COMMAND, IDC_CHKSTATUS, himsw::force_as<LPARAM>(chk_hwnd));

        return window;
    }

    explicit dialog_window(passkey) {
        auto instance = ::GetModuleHandleW(nullptr);
        dlg_ = ::CreateDialogParamW(instance,
                                    MAKEINTRESOURCE(IDD_DLGMAIN),
                                    nullptr,
                                    &dialog_window_manager::dialog_proc,
                                    0);
        if (!dlg_) {
            throw himsw::win_last_error("Failed to create dialog window");
        }

        // Leak on purpose
        auto hicon = ::LoadIconW(instance, MAKEINTRESOURCE(IDI_ICON));
        if (!hicon) {
            throw himsw::win_last_error("Failed to load application icon");
        }

        tray_ = std::make_unique<himsw::tray>(dlg_, IDI_ICON, hicon, k_tray_msgid, L"himsw");

        events_[WM_CLOSE] = [this](WPARAM, LPARAM) {
            on_close();
        };

        events_[WM_SIZE] = [this](WPARAM wparam, LPARAM) {
            if (wparam == SIZE_MINIMIZED) {
                on_minimized();
            }
        };

        events_[WM_TIMER] = [](WPARAM, LPARAM) {
            // currently only one timer.
            on_timer();
        };

        events_[k_tray_msgid] = [this](WPARAM wparam, LPARAM lparam) {
            auto icon_id = static_cast<UINT>(wparam);
            auto mouse_msg = static_cast<UINT>(lparam);
            if (icon_id == IDI_ICON) {
                on_tray_icon(mouse_msg);
            }
        };

        events_[WM_COMMAND] = [this](WPARAM wparam, LPARAM lparam) {
            auto ctrl_id = LOWORD(wparam);
            if (ctrl_id == IDC_CHKSTATUS) {
                auto state = Button_GetCheck(himsw::force_as<HWND>(lparam));
                on_monitor_state_changed(state == BST_CHECKED);
            }
        };
    }

    void show(bool visible = true) {
        auto cmd = visible ? SW_SHOWNORMAL : SW_HIDE;
        ::ShowWindow(dlg_, cmd);
        visible_ = visible;
    }

    bool visible() const noexcept {
        return visible_;
    }

    bool try_process_event(unsigned int event_id, WPARAM wparam, LPARAM lparam) {
        auto entry = events_.find(event_id);
        if (entry == events_.end()) {
            return false;
        }

        entry->second(wparam, lparam);

        return true;
    }

    void update_info(const std::string& msg) {
        msgs_.push_front(now_in_date() + msg);
        if (msgs_.size() > k_max_kept_msgs) {
            msgs_.pop_back();
        }

        auto text = join_string(msgs_.begin(), msgs_.end(), "\r\n");
        ::SetDlgItemTextA(dlg_, IDC_MSG, text.c_str());
    }

private:
    void on_close() {
        tray_ = nullptr;
        ::KillTimer(dlg_, IDT_TIMER);
        ::DestroyWindow(dlg_);
        dialog_window_manager::instance().unroll(dlg_);
    }

    void on_minimized() {
        show(!visible());
    }

    static void on_timer() {
        himsw::labor_monitor::instance().tick();
    }

    void on_tray_icon(UINT mouse_msg) {
        switch (mouse_msg) {
        case WM_LBUTTONDBLCLK:
            show(!visible());
            break;
        default:
            break;
        }
    }

    void on_monitor_state_changed(bool enabled) {
        if (enabled) {
            ::SetTimer(dlg_, IDT_TIMER, 1000 * 10, nullptr);
            update_info("monitoring is on");
        } else {
            ::KillTimer(dlg_, IDT_TIMER);
            update_info("monitoring is off");
        }
    }

private:
    HWND dlg_{nullptr};
    bool visible_{false};
    std::unordered_map<unsigned int, std::function<void(WPARAM, LPARAM)>> events_;
    std::unique_ptr<himsw::tray> tray_;
    std::deque<std::string> msgs_;
    static constexpr size_t k_max_kept_msgs{10};
    static constexpr int k_tray_msgid{WM_USER + 10};
};

INT_PTR dialog_window_manager::dialog_proc(HWND dlg, UINT msg, WPARAM wparam, LPARAM lparam) {
    auto& mgr = dialog_window_manager::instance();
    auto it = mgr.dlg_table_.find(dlg);
    if (it != mgr.dlg_table_.end() && it->second->try_process_event(msg, wparam, lparam)) {
        return TRUE;
    }
    return FALSE;
}

class main_loop {
public:
    main_loop() = default;

    ~main_loop() = default;

    main_loop(const main_loop&) = delete;

    main_loop(main_loop&&) = delete;

    main_loop& operator=(const main_loop&) = delete;

    main_loop& operator=(main_loop&&) = delete;

    static void run() {
        MSG msg;
        while (::GetMessageW(&msg, nullptr, 0, 0) != 0) {
            if (!dialog_window_manager::instance().any_dialog_message_processed(&msg)) {
                ::TranslateMessage(&msg);
                ::DispatchMessageW(&msg);
            }
        }
    }
};

} // namespace

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    constexpr int critical_failure = 1;

    ::SetProcessDPIAware();

    try {
        spdlog::set_default_logger(spdlog::basic_logger_mt("main_logger", "himsw.log"));
        spdlog::default_logger()->flush_on(spdlog::level::info);

        tm break_start{};
        tm break_stop{};
        break_start.tm_hour = 11;
        break_start.tm_min = 30;
        break_stop.tm_hour = 13;
        break_stop.tm_min = 10;
        himsw::monitor_config cfg;
        cfg.break_ranges.emplace_back(break_start, break_stop);
        himsw::labor_monitor::instance().update_config(cfg);
        himsw::labor_monitor::instance().prepare();

        auto main_window = dialog_window::make();
        himsw::labor_monitor::instance().set_info_update_handler(
                [ptr = std::weak_ptr(main_window)](const std::string& msg) {
                    if (auto wnd = ptr.lock(); wnd) {
                        wnd->update_info(msg);
                    }
                });

        main_loop::run();
    } catch (const spdlog::spdlog_ex& ex) {
        auto reason = fmt::format("Failed to initialize logging component; ex={}", ex.what());
        ::MessageBoxA(nullptr, reason.c_str(), "Error", MB_ICONERROR);
        return critical_failure;
    } catch (const himsw::win_last_error& ex) {
        const std::string cause = "Failed to prepare the labor monitor";
        spdlog::error("{}; ex={} ec={}", cause, ex.what(), ex.error_code());
        ::MessageBoxA(nullptr, cause.c_str(), "Error", MB_ICONERROR);
        return critical_failure;
    }

    return 0;
}
