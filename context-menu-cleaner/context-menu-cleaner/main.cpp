#include <Windows.h>

#include "main_window.h"

int main()
{
    SetProcessDPIAware();

    auto main_window = MainWindow::Make();
    nana::exec();
    return 0;
}
