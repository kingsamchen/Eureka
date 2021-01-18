#include <Windows.h>

#include "main_window.h"

int main()
{
    SetProcessDPIAware();

    auto main_window = main_window::make();
    nana::exec();

    return 0;
}
