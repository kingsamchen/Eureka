#ifndef CONTEXT_MENU_CLEANER_SHELL_MENU_H_
#define CONTEXT_MENU_CLEANER_SHELL_MENU_H_

#include <string>
#include <vector>

#include <Windows.h>

namespace shell_ext {

struct MenuEntry {
    using string_type = std::wstring;

    string_type path;
    string_type value;
    string_type command;
};

void ScanEntriesAt(HKEY root, const wchar_t* sub, std::vector<MenuEntry>& entries);

}   // namespace cmc

#endif  // CONTEXT_MENU_CLEANER_SHELL_MENU_H_
