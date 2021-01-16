#ifndef CONTEXT_MENU_CLEANER_MAIN_WINDOW_H_
#define CONTEXT_MENU_CLEANER_MAIN_WINDOW_H_

#include "nana/gui.hpp"
#include "nana/gui/widgets/listbox.hpp"

class MainWindow : public nana::form {
public:
    MainWindow()
        : nana::form(nana::API::make_center(2373, 1335))
    {
        listbox_.append_header(L"Path");
        listbox_.append_header(L"Value");
        listbox_.append_header(L"Command");

        place_.div("<list>");
        place_["list"] << listbox_;
    }

private:
    nana::listbox listbox_ {*this, nana::rectangle(this->size())};
    nana::place place_ {*this};
};

#endif  // CONTEXT_MENU_CLEANER_MAIN_WINDOW_H_
