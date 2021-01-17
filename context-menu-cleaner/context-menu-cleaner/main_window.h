#ifndef CONTEXT_MENU_CLEANER_MAIN_WINDOW_H_
#define CONTEXT_MENU_CLEANER_MAIN_WINDOW_H_

#include <functional>
#include <memory>
#include <tuple>

#include "nana/gui.hpp"
#include "nana/gui/widgets/listbox.hpp"
#include "nana/gui/widgets/menu.hpp"

#include "kbase/basic_macros.h"

#include "shell_menu.h"

namespace internal {

inline nana::size CalcInitialMainWindowSize()
{
    constexpr auto kRatio = 0.618;
    auto monitor = nana::screen::primary_monitor_size();
    auto init_width = static_cast<nana::size::value_type>(monitor.width * kRatio + 0.5);
    auto init_height = static_cast<nana::size::value_type>(monitor.height * kRatio + 0.5);
    return {init_width, init_height};
}

}   // namespace internal

inline nana::listbox::oresolver& operator<<(nana::listbox::oresolver& out,
                                            const shell_ext::MenuEntry& e)
{
    out << e.value
        << e.path
        << e.command;
    return out;
}

inline std::ostream& operator<< (std::ostream& out, const shell_ext::MenuEntry& e)
{
    out << nana::charset(e.value).to_bytes(nana::unicode::utf8)
        << nana::charset(e.path).to_bytes(nana::unicode::utf8)
        << nana::charset(e.command).to_bytes(nana::unicode::utf8);
    return out;
}

inline nana::listbox::iresolver& operator>>(nana::listbox::iresolver& in, shell_ext::MenuEntry& e)
{
    in >> e.value
       >> e.path
       >> e.command;
    return in;
}

class MainWindow : public nana::form {
    struct PassKey {
        explicit PassKey() = default;
    };

public:
    static std::unique_ptr<MainWindow> Make()
    {
        auto initial = internal::CalcInitialMainWindowSize();
        auto window = std::make_unique<MainWindow>(PassKey{}, initial.width, initial.height);
        window->show();
        window->LoadMenuItems();
        return window;
    }

    MainWindow(PassKey, unsigned int width, unsigned int height)
        : nana::form(nana::API::make_center(width, height))
    {
        auto [len0, len1, len2] = CalcColumnLength(width);
        listbox_.append_header("Menu", len0);
        listbox_.append_header("Path", len1);
        listbox_.append_header("Command", len2);

        listbox_.events().mouse_down(nana::menu_popuper(popup_menu_, nana::mouse::right_button));

        popup_menu_.append("Delete selected",
                           std::bind(&MainWindow::OnDeleteItem, this, std::placeholders::_1));
        popup_menu_.append_splitter();

        place_.div("<list>");
        place_["list"] << listbox_;
        place_.collocate();

        this->caption("The Stupid Context-Menu Cleaner");

        events().resized(std::bind(&MainWindow::OnResized, this, std::placeholders::_1));
    }

    void LoadMenuItems()
    {
        const std::tuple<HKEY, const wchar_t*> locations[] {
            {HKEY_CLASSES_ROOT, L"*\\shell"},
            {HKEY_CLASSES_ROOT, L"Directory\\shell"},
            {HKEY_CLASSES_ROOT, L"Directory\\Background\\shell"}
        };

        std::vector<shell_ext::MenuEntry> menu_entries;
        for (const auto& [root, subkey] : locations) {
            shell_ext::ScanEntriesAt(root, subkey, menu_entries);
        }

        listbox_.clear();
        for (const auto& item : menu_entries) {
            listbox_.at(0).append(item);
        }
    }

private:
    void OnResized(const nana::arg_resized& arg)
    {
        auto [col0_len, col1_len, col2_len] = CalcColumnLength(arg.width);
        listbox_.column_at(0).width(col0_len);
        listbox_.column_at(1).width(col1_len);
        listbox_.column_at(2).width(col2_len);
    }

    void OnDeleteItem([[maybe_unused]] const nana::menu::item_proxy& ip)
    {
        auto pos = listbox_.selected().data()->item;
        shell_ext::MenuEntry entry;
        listbox_.at(0).at(pos).resolve_to(entry);
        try {
            shell_ext::DeleteEntry(entry.path);
        } catch (const std::exception& ex) {
            nana::msgbox mb(*this, ex.what());
            IGNORE_RESULT(mb.show());
        }
        LoadMenuItems();
    }

    static std::tuple<uint32_t, uint32_t, uint32_t> CalcColumnLength(unsigned int window_width)
    {
        return {window_width / 5, window_width / 3, window_width / 2};
    }

private:
    nana::listbox listbox_ {*this, nana::rectangle(this->size())};
    nana::menu popup_menu_;
    nana::place place_ {*this};
};

#endif  // CONTEXT_MENU_CLEANER_MAIN_WINDOW_H_
