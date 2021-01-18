#include "shell_menu.h"

#include "kbase/registry.h"
#include "kbase/string_format.h"
#include "kbase/string_util.h"

namespace shell_ext {

std::wstring stringify_root_key(HKEY root)
{
    // Workaround for switch on pointers.
    constexpr auto k_class_root = reinterpret_cast<ULONG_PTR>(HKEY_CLASSES_ROOT);
    constexpr auto k_current_user = reinterpret_cast<ULONG_PTR>(HKEY_CURRENT_USER);
    constexpr auto k_local_machine = reinterpret_cast<ULONG_PTR>(HKEY_LOCAL_MACHINE);
    constexpr auto k_users = reinterpret_cast<ULONG_PTR>(HKEY_USERS);

    switch (reinterpret_cast<ULONG_PTR>(root)) {
    case k_class_root:
        return L"HKEY_CLASSES_ROOT";
    case k_current_user:
        return L"HKEY_CURRENT_USER";
    case k_local_machine:
        return L"HKEY_LOCAL_MACHINE";
    case k_users:
        return L"HKEY_USERS";
    default:
        throw std::invalid_argument(kbase::StringPrintf(
            "unknown root key; key=%x", reinterpret_cast<ULONG_PTR>(root)));
    }
}

HKEY root_key_from_string(std::wstring_view root)
{
    if (root == L"HKEY_CLASSES_ROOT") {
        return HKEY_CLASSES_ROOT;
    }

    if (root == L"HKEY_CURRENT_USER") {
        return HKEY_CURRENT_USER;
    }

    if (root == L"HKEY_LOCAL_MACHINE") {
        return HKEY_LOCAL_MACHINE;
    }

    if (root == L"HKEY_USERS") {
        return HKEY_USERS;
    }

    auto key = kbase::WideToUTF8(std::wstring(root));
    throw std::invalid_argument(
        kbase::StringPrintf("unknown root key string; key=%s", key.c_str()));
}

std::wstring append_reg_path(const std::wstring& base, const std::wstring& key)
{
    std::wstring result(base);
    if (!kbase::EndsWith(result, L"\\")) {
        result.append(L"\\");
    }

    result.append(key);
    return result;
}

void scan_entry_at(HKEY root, const wchar_t* sub, std::vector<menu_entry>& entries)
{
    auto prefix = append_reg_path(stringify_root_key(root), sub);

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
        } else {
            value = key.subkey_name();
        }

        command.ReadValue(L"", cmd_value);
        auto ent = menu_entry{append_reg_path(prefix, key.subkey_name()), value, cmd_value};
        entries.push_back(std::move(ent));
    }
}

void delete_entry(const std::wstring& key_path)
{
    auto slash_pos = key_path.find(L'\\');
    if (slash_pos == std::wstring::npos) {
        auto path = kbase::WideToUTF8(kbase::WStringView(key_path));
        throw std::invalid_argument(kbase::StringPrintf("invalid key path; path=%s", path.c_str()));
    }

    auto root = root_key_from_string(key_path.substr(0, slash_pos));

    auto last_slash_pos = key_path.rfind(L'\\');
    auto subpath = key_path.substr(slash_pos + 1, last_slash_pos - slash_pos - 1);
    auto entry = key_path.substr(last_slash_pos + 1);

    kbase::RegKey key;
    key.Open(root, subpath.c_str(), KEY_WRITE);
    if (!key) {
        throw std::runtime_error("cannot open the key");
    }

    key.DeleteKey(entry.c_str());
}

}   // namespace shell_ext
