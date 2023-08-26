// Copyright (c) 2021 Kingsley Chen <kingsamchen at gmail dot com>
// This file is subject to the terms of license that can be found
// in the LICENSE file.

#pragma once

#ifndef HIMSW_LABOR_MONITOR_H_
#define HIMSW_LABOR_MONITOR_H_

#include <chrono>
#include <functional>
#include <string>
#include <utility>

#include <Windows.h>

namespace himsw {

using daytime_range = std::pair<tm, tm>;

struct monitor_config {
    std::chrono::seconds max_idle{180};
    DWORD simulation_key{VK_SCROLL};
    std::vector<daytime_range> break_ranges;
};

template<typename To, typename From>
To force_as(From from) {
    return reinterpret_cast<To>(from); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
}

class labor_monitor {
private:
    enum class state {
        active,
        simulating
    };

public:
    // Would automatically unhook if any installed.
    ~labor_monitor();

    labor_monitor(const labor_monitor&) = delete;

    labor_monitor(labor_monitor&&) = delete;

    labor_monitor& operator=(const labor_monitor&) = delete;

    labor_monitor& operator=(labor_monitor&&) = delete;

    static labor_monitor& instance();

    void update_config(const monitor_config& cfg) {
        cfg_ = cfg;
    }

    // May throw win_last_error
    void prepare();

    void tick();

    void set_info_update_handler(std::function<void(const std::string&)>&& fn) {
        info_handler_ = std::move(fn);
    }

    void enable_breakoff(bool enable);

private:
    labor_monitor()
        : state_(state::active),
          keyboard_hook_(nullptr),
          mouse_hook_(nullptr),
          info_handler_(noop) {}

    void cleanup() noexcept;

    bool is_simulating() const noexcept {
        return state_ == state::simulating;
    }

    // May throw win_last_error
    static std::chrono::seconds get_idle_duration();

    // May throw win_last_error
    void tick_on_active(bool within_breakoff);

    // May throw win_last_error
    void tick_on_simulating(bool within_breakoff);

    static LRESULT keyboard_hook_proc(int code, WPARAM wparam, LPARAM lparam);

    static LRESULT mouse_hook_proc(int code, WPARAM wparam, LPARAM lparam);

    static void noop(const std::string&) {}

private:
    state state_;
    bool breakoff_enabled_{false};
    std::chrono::system_clock::time_point last_simulation_;
    static constexpr std::chrono::seconds k_simul_interval{30};
    monitor_config cfg_;
    HHOOK keyboard_hook_;
    HHOOK mouse_hook_;
    std::function<void(const std::string&)> info_handler_;
};

} // namespace himsw

#endif // HIMSW_LABOR_MONITOR_H_
