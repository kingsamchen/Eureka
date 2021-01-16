#include "shell_menu.h"

#include "kbase/registry.h"
#include "kbase/string_format.h"
#include "kbase/string_util.h"

namespace shell_ext {

std::wstring StringifyRootKey(HKEY root)
{
    // Workaround for switch on pointers.
    constexpr auto kClassRoot = reinterpret_cast<ULONG_PTR>(HKEY_CLASSES_ROOT);
    constexpr auto kCurrentUser = reinterpret_cast<ULONG_PTR>(HKEY_CURRENT_USER);
    constexpr auto kLocalMachine = reinterpret_cast<ULONG_PTR>(HKEY_LOCAL_MACHINE);
    constexpr auto kUsers = reinterpret_cast<ULONG_PTR>(HKEY_USERS);

    switch (reinterpret_cast<ULONG_PTR>(root)) {
    case kClassRoot:
        return L"HKEY_CLASSES_ROOT";
    case kCurrentUser:
        return L"HKEY_CURRENT_USER";
    case kLocalMachine:
        return L"HKEY_LOCAL_MACHINE";
    case kUsers:
        return L"HKEY_USERS";
    default:
        throw std::invalid_argument(kbase::StringPrintf(
            "unknown root key; key=%x", reinterpret_cast<ULONG_PTR>(root)));
    }
}

std::wstring AppendRegPath(const std::wstring& base, const std::wstring& key)
{
    std::wstring result(base);
    if (!kbase::EndsWith(result, L"\\")) {
        result.append(L"\\");
    }

    result.append(key);
    return result;
}

void ScanEntriesAt(HKEY root, const wchar_t* sub, std::vector<MenuEntry>& entries)
{
    auto prefix = AppendRegPath(StringifyRootKey(root), sub);

    kbase::RegKeyIterator key_it(root, sub);
    for (const auto& key : key_it) {
        if (!key) {
            continue;
        }

        kbase::RegKey command;
        command.Open(key.Get(), L"command", KEY_READ);
        if (!command || !command.HasValue(L"")) {
            continue;
        }

        std::wstring value, cmd_value;
        if (key.HasValue(L"")) {
            key.ReadValue(L"", value);
        }

        command.ReadValue(L"", cmd_value);
        auto ent = MenuEntry{AppendRegPath(prefix, key.subkey_name()), value, cmd_value};
        entries.push_back(std::move(ent));
    }
}

}   // namespace shell_ext
