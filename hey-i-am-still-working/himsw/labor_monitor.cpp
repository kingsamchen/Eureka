// Copyright (c) 2021 Kingsley Chen <kingsamchen at gmail dot com>
// This file is subject to the terms of license that can be found
// in the LICENSE file.

#include "himsw/labor_monitor.h"

#include <cassert>
#include <thread>

#include "spdlog/spdlog.h"

#include "himsw/win_last_error.h"

namespace himsw {
namespace {

constexpr ULONG_PTR k_simulation_mask = 0xdeadbeef;

template<typename T, size_t N>
constexpr size_t array_size(T (&)[N]) {
    return N;
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
}

void labor_monitor::cleanup() noexcept {
    if (keyboard_hook_) {
        ::UnhookWindowsHookEx(keyboard_hook_);
        keyboard_hook_ = nullptr;
    }
}

void labor_monitor::tick() {
    try {
        if (state_ == state::active) {
            tick_on_active();
        } else {
            tick_on_simulating();
        }
    } catch (const win_last_error& ex) {
        spdlog::error("Failed to handle the tick; ec={} what={}", ex.error_code(), ex.what());
    }
}

void labor_monitor::tick_on_active() {
    assert(state_ == state::active);
    auto idle = get_idle_duration();
    info_handler_(fmt::format("Being idle for {} seconds", idle.count()));
    if (idle > cfg_.max_idle) {
        spdlog::info("Monitor state changed; active -> simulating");
        state_ = state::simulating;
        tick_on_simulating();
    }
}

void labor_monitor::tick_on_simulating() const {
    assert(state_ == state::simulating);

    INPUT key_inputs[2]{};
    // Key press
    key_inputs[0].type = INPUT_KEYBOARD;
    key_inputs[0].ki.wVk = VK_SCROLL;
    key_inputs[0].ki.dwExtraInfo = k_simulation_mask;
    // key released
    key_inputs[1].type = INPUT_KEYBOARD;
    key_inputs[1].ki.wVk = VK_SCROLL;
    key_inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    key_inputs[1].ki.dwExtraInfo = k_simulation_mask;

    auto input_size = static_cast<UINT>(array_size(key_inputs));
    auto sent_cnt = ::SendInput(input_size, key_inputs, sizeof(INPUT));
    if (sent_cnt != input_size) {
        throw win_last_error("call SendInput()");
    }

    info_handler_("simulation key events emitted");
}

std::chrono::seconds labor_monitor::get_idle_duration() const {
    LASTINPUTINFO info{};
    info.cbSize = sizeof(info);
    if (BOOL ok = ::GetLastInputInfo(&info); !ok) {
        throw win_last_error("call GetLastInputInfo()");
    }

    ULONGLONG cur_tick = ::GetTickCount64();
    auto dur = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::milliseconds(cur_tick - static_cast<ULONGLONG>(info.dwTime)));
    return dur;
}

// static
LRESULT labor_monitor::keyboard_hook_proc(int code, WPARAM wparam, LPARAM lparam) {
    auto& monitor = labor_monitor::instance();
    if (code >= 0 && monitor.is_simulating()) {
        auto key_event = reinterpret_cast<PKBDLLHOOKSTRUCT>(lparam);
        // The key event is not from our simulation.
        if (key_event->vkCode != monitor.cfg_.simulation_key ||
            key_event->dwExtraInfo != k_simulation_mask) {
            monitor.state_ = state::active;
            spdlog::info("Monitor state changed; simulating -> active");
            monitor.info_handler_("User keyboard activities detected");
        }
    }
    return ::CallNextHookEx(monitor.keyboard_hook_, code, wparam, lparam);
}

} // namespace himsw
