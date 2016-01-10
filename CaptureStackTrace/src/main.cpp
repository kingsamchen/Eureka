/*
 @ 0xCCCCCCCC
*/

#include <conio.h>

#include <iostream>

#include "stack_walker.h"

void CaptureCallStack()
{
    StackWalker callstack;
    callstack.OutputCallStack(std::cout);
}

void Func3(int& val)
{
    ++val;
    std::cout << val;
    CaptureCallStack();
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