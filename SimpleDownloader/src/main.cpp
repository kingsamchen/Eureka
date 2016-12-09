/*
 @ 0xCCCCCCCC
*/

#include "base/at_exit.h"
#include "base/command_line.h"

int main(int argc, char* argv[])
{
    CommandLine::Init(0, nullptr);
    base::AtExitManager exit_manager;

    return 0;
}