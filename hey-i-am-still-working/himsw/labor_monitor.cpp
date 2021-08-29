// Copyright (c) 2021 Kingsley Chen <kingsamchen at gmail dot com>
// This file is subject to the terms of license that can be found
// in the LICENSE file.

#include "himsw/labor_monitor.h"

#include <cassert>

#include <Windows.h>

#include "himsw/win_last_error.h"

namespace himsw {
namespace {

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

void labor_monitor::tick() {
    if (state_ == state::active) {
        tick_on_active();
    } else {
        tick_on_simulating();
    }
}

void labor_monitor::tick_on_active() {
    assert(state_ == state::active);
    if (auto idle = get_idle_duration(); idle > cfg_.max_idle) {
        fprintf_s(stderr, "Monitor state changed; active -> simulating\n");
        state_ = state::simulating;
        tick_on_simulating();
    }
}

void labor_monitor::tick_on_simulating() {
    assert(state_ == state::simulating);

    INPUT key_inputs[2]{};
    // Key press
    key_inputs[0].type = INPUT_KEYBOARD;
    key_inputs[0].ki.wVk = VK_SCROLL;
    // key released
    key_inputs[1].type = INPUT_KEYBOARD;
    key_inputs[1].ki.wVk = VK_SCROLL;
    key_inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    auto input_size = static_cast<UINT>(array_size(key_inputs));
    auto sent_cnt = ::SendInput(input_size, key_inputs, sizeof(INPUT));
    if (sent_cnt != input_size) {
        fprintf_s(stderr, "Failed to SendInput(); ec=%ul\n", ::GetLastError());
    }
}

std::chrono::seconds labor_monitor::get_idle_duration() const {
    LASTINPUTINFO info{};
    info.cbSize = sizeof(info);
    if (BOOL ok = ::GetLastInputInfo(&info); !ok) {
        throw win_last_error("call GetLastInputInfo()");
    }

    ULONGLONG cur_tick = ::GetTickCount64();
    auto dur = std::chrono::milliseconds(cur_tick - static_cast<ULONGLONG>(info.dwTime));
    fprintf_s(stderr, "Idle for %lld seconds\n", dur.count());
    return std::chrono::duration_cast<std::chrono::seconds>(dur);
}

} // namespace himsw
