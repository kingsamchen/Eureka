#include <winuser.h>

#include "main_window.h"

int main()
{
    SetProcessDPIAware();

    MainWindow window;
    window.show();
    nana::exec();

    return 0;
}
