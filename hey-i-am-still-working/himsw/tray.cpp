// Copyright (c) 2021 Kingsley Chen <kingsamchen at gmail dot com>
// This file is subject to the terms of license that can be found
// in the LICENSE file.

#include "himsw/tray.h"

namespace himsw {

tray::tray(HWND main_window, UINT icon_id, HICON icon, UINT tray_msgid, const std::wstring& tip) {
    icon_data_.cbSize = sizeof(icon_data_);
    icon_data_.hWnd = main_window;
    icon_data_.uID = icon_id;
    icon_data_.hIcon = icon;
    icon_data_.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    icon_data_.uCallbackMessage = tray_msgid;
    wcscpy_s(icon_data_.szTip, tip.c_str());

    ::Shell_NotifyIconW(NIM_ADD, &icon_data_);
}

tray::~tray() {
    ::Shell_NotifyIconW(NIM_DELETE, &icon_data_);
}

} // namespace himsw
