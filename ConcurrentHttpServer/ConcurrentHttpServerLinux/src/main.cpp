/*
 @ 0xCCCCCCCC
*/

#include <stdio.h>

#include "kbase/at_exit_manager.h"
#include "kbase/command_line.h"

int main(int argc, char* argv[])
{
    kbase::CommandLine::Init(argc, argv);
    kbase::AtExitManager exit_manager;

    printf("hello world!");

    return 0;
}
