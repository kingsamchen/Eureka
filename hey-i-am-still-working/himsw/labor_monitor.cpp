// Copyright (c) 2021 Kingsley Chen <kingsamchen at gmail dot com>
// This file is subject to the terms of license that can be found
// in the LICENSE file.

#include "himsw/labor_monitor.h"

#include <algorithm>
#include <cassert>
#include <thread>

#include "spdlog/spdlog.h"

#include "himsw/win_last_error.h"

namespace himsw {
namespace {

constexpr ULONG_PTR k_simulation_mask = 0xdeadbeef;

int compare(const tm& lhs, const tm& rhs) {
    if (lhs.tm_hour < rhs.tm_hour) {
        return -1;
    }

    if (lhs.tm_hour > rhs.tm_hour) {
        return 1;
    }

    return lhs.tm_min - rhs.tm_min;
}

} // namespace

// static
labor_monitor& labor_monitor::instance() {
    static labor_monitor instance;
    return instance;
}

labor_monitor::~labor_monitor() {
    cleanup();
}

void labor_monitor::prepare() {
    auto module = ::GetModuleHandleW(nullptr);

    keyboard_hook_ = ::SetWindowsHookExW(WH_KEYBOARD_LL, &labor_monitor::keyboard_hook_proc,
                                         module, 0);
    if (!keyboard_hook_) {
        throw win_last_error("install keyboard hook");
    }

    spdlog::info("Prepare: low-level keyboard hook has been installed");

    mouse_hook_ = ::SetWindowsHookExW(WH_MOUSE_LL, &labor_monitor::mouse_hook_proc, module, 0);
    if (!mouse_hook_) {
        throw win_last_error("install mouse hook");
    }

    spdlog::info("Prepare: low-level mouse hook has been installed");
}

void labor_monitor::cleanup() noexcept {
    if (keyboard_hook_) {
        ::UnhookWindowsHookEx(keyboard_hook_);
        keyboard_hook_ = nullptr;
    }

    if (mouse_hook_) {
        ::UnhookWindowsHookEx(mouse_hook_);
        mouse_hook_ = nullptr;
    }
}

void labor_monitor::tick() {
    auto tp = std::time(nullptr);
    tm now_tm{};
    localtime_s(&now_tm, &tp);
    auto within_breakoff =
            std::any_of(cfg_.break_ranges.begin(), cfg_.break_ranges.end(),
                        [&now_tm](const daytime_range& breakoff_range) {
                            const auto& [st, ed] = breakoff_range;
                            // Not so restrict.
                            return compare(st, ed) <= 0
                                           ? (compare(now_tm, st) >= 0 && compare(now_tm, ed) <= 0)
                                           : (compare(now_tm, st) >= 0 || compare(now_tm, ed) <= 0);
                        });
    try {
        if (state_ == state::active) {
            tick_on_active(within_breakoff);
        } else {
            tick_on_simulating(within_breakoff);
        }
    } catch (const win_last_error& ex) {
        spdlog::error("Failed to handle the tick; ec={} what={}", ex.error_code(), ex.what());
    }
}

void labor_monitor::tick_on_active(bool within_breakoff) {
    assert(state_ == state::active);
    auto idle = get_idle_duration();
    info_handler_(fmt::format("Being idle for {} seconds", idle.count()));
    if (idle > cfg_.max_idle) {
        if (within_breakoff) {
            const std::string supress_msg = "Within breakoff time, suppress simulating";
            info_handler_(supress_msg);
            return;
        }

        spdlog::info("Monitor state changed; active -> simulating");
        state_ = state::simulating;
        tick_on_simulating(false);
    }
}

void labor_monitor::tick_on_simulating(bool within_breakoff) {
    assert(state_ == state::simulating);

    if (within_breakoff) {
        info_handler_("Wihtin breakoff time, stop simulating");
        state_ = state::active;
        return;
    }

    auto now = std::chrono::system_clock::now();
    if ((now - last_simulation_) < labor_monitor::k_simul_interval) {
        return;
    }

    INPUT key_inputs[2]{};
    // NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)
    // Key press
    key_inputs[0].type = INPUT_KEYBOARD;
    key_inputs[0].ki.wVk = VK_SCROLL;
    key_inputs[0].ki.dwExtraInfo = k_simulation_mask;
    // key released
    key_inputs[1].type = INPUT_KEYBOARD;
    key_inputs[1].ki.wVk = VK_SCROLL;
    key_inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    key_inputs[1].ki.dwExtraInfo = k_simulation_mask;
    // NOLINTEND(cppcoreguidelines-pro-type-union-access)

    auto input_size = static_cast<UINT>(std::size(key_inputs));
    auto sent_cnt = ::SendInput(input_size, key_inputs, sizeof(INPUT));
    if (sent_cnt != input_size) {
        throw win_last_error("call SendInput()");
    }

    last_simulation_ = now;

    info_handler_("simulation key events emitted");
}

// static
std::chrono::seconds labor_monitor::get_idle_duration() {
    LASTINPUTINFO info{};
    info.cbSize = sizeof(info);
    if (const BOOL ok = ::GetLastInputInfo(&info); !ok) {
        throw win_last_error("call GetLastInputInfo()");
    }

    const ULONGLONG cur_tick = ::GetTickCount64();
    auto dur = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::milliseconds(cur_tick - static_cast<ULONGLONG>(info.dwTime)));
    return dur;
}

// static
LRESULT labor_monitor::keyboard_hook_proc(int code, WPARAM wparam, LPARAM lparam) {
    auto& monitor = labor_monitor::instance();
    if (code >= 0 && monitor.is_simulating()) {
        auto key_event = force_as<PKBDLLHOOKSTRUCT>(lparam);
        // The key event is not from our simulation.
        if (key_event->vkCode != monitor.cfg_.simulation_key ||
            key_event->dwExtraInfo != k_simulation_mask) {
            monitor.state_ = state::active;
            spdlog::info("Monitor state changed due to keyboard activity; simulating -> active");
            monitor.info_handler_("User keyboard activities detected");
        }
    }

    return ::CallNextHookEx(monitor.keyboard_hook_, code, wparam, lparam);
}

// static
LRESULT labor_monitor::mouse_hook_proc(int code, WPARAM wparam, LPARAM lparam) {
    auto& monitor = labor_monitor::instance();
    if (code >= 0 && monitor.is_simulating()) {
        monitor.state_ = state::active;
        spdlog::info("Monitor state changed due to mouse activity; simulating -> active");
        monitor.info_handler_("User mouse activities detected");
    }

    return ::CallNextHookEx(monitor.mouse_hook_, code, wparam, lparam);
}

} // namespace himsw
