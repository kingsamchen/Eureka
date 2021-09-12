// Copyright (c) 2021 Kingsley Chen <kingsamchen at gmail dot com>
// This file is subject to the terms of license that can be found
// in the LICENSE file.

#pragma once

#ifndef HIMSW_TRAY_H_
#define HIMSW_TRAY_H_

#include <string>

#include <Windows.h>

#include <shellapi.h>

namespace himsw {

class tray {
public:
    tray(HWND main_window, UINT icon_id, HICON icon, UINT tray_msgid, const std::wstring& tip);

    ~tray();

    tray(const tray&) = delete;

    tray(tray&&) = delete;

    tray& operator=(const tray&) = delete;

    tray& operator=(tray&&) = delete;

private:
    NOTIFYICONDATAW icon_data_{};
};

} // namespace himsw

#endif // HIMSW_TRAY_H_
