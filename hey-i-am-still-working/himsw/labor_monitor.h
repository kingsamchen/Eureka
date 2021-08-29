// Copyright (c) 2021 Kingsley Chen <kingsamchen at gmail dot com>
// This file is subject to the terms of license that can be found
// in the LICENSE file.

#pragma once

#ifndef HIMSW_LABOR_MONITOR_H_
#define HIMSW_LABOR_MONITOR_H_

#include <chrono>

namespace himsw {

struct monitor_config {
    std::chrono::seconds max_idle{180};
};

class labor_monitor {
private:
    enum class state {
        active,
        simulating
    };

public:
    ~labor_monitor() = default;

    labor_monitor(const labor_monitor&) = delete;

    labor_monitor(labor_monitor&&) = delete;

    labor_monitor& operator=(const labor_monitor&) = delete;

    labor_monitor& operator=(labor_monitor&&) = delete;

    static labor_monitor& instance();

    void update_config(const monitor_config& cfg) {
        cfg_ = cfg;
    }

    // star()

    // stop()

    void tick();

private:
    labor_monitor()
        : state_(state::active) {}

    // May throw win_last_error
    std::chrono::seconds get_idle_duration() const;

    void tick_on_active();

    void tick_on_simulating();

private:
    state state_;
    monitor_config cfg_;
};

} // namespace himsw

#endif // HIMSW_LABOR_MONITOR_H_
