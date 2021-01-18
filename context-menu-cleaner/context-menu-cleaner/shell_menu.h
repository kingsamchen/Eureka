#ifndef CONTEXT_MENU_CLEANER_SHELL_MENU_H_
#define CONTEXT_MENU_CLEANER_SHELL_MENU_H_

#include <string>
#include <vector>

#include <Windows.h>

namespace shell_ext {

struct menu_entry {
    using string_type = std::wstring;

    string_type path;
    string_type value;
    string_type command;
};

void scan_entry_at(HKEY root, const wchar_t* sub, std::vector<menu_entry>& entries);
void delete_entry(const std::wstring& key_path);

}   // namespace shell_ext

#endif  // CONTEXT_MENU_CLEANER_SHELL_MENU_H_
