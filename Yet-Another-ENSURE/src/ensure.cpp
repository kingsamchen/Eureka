/*
 @ 0xCCCCCCCC
*/

#include "ensure.h"

#include <iostream>

namespace {

bool g_always_check_in_debug_mode = true;

}   // namespace

void Guarantor::Require() const
{
    switch (action_required_) {
    case Action::CHECK:
        Check();
        break;
    case Action::RAISE:
        Raise();
        break;
    case Action::RAISE_WITH_DUMP:
        RaiseWithDump();
        break;
    }
}

void Guarantor::Check() const
{
    (void)exception_desc_;
    __debugbreak();
}

void Guarantor::Raise() const
{
    (void)exception_desc_;
    std::cout << "-> Raise()\n";
    std::cout << exception_desc_.str();
}

void Guarantor::RaiseWithDump() const
{
    (void)exception_desc_;
    std::cout << "-> RaiseWithDump()\n";
}