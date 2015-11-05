/*
 @ 0xCCCCCCCC
*/

#include "signals.h"

int main()
{
    Signal<int, const int&, int&&> s;
    s.Emit(1, 3, 5);
    return 0;
}