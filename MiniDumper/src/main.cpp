/*
 @ 0xCCCCCCCC
*/

#include <conio.h>

#include "minidumper.h"

void Dump()
{
    CreateMiniDump(L"dump_test.dmp");
}

void Func3(int& val)
{
    ++val;
    Dump();
}

void Func2(int& val)
{
    ++val;
    Func3(val);
}

void Func1(int& val)
{
    ++val;
    Func2(val);
}

int main()
{
    int i = 0;
    Func1(i);
    _getch();
    return 0;
}