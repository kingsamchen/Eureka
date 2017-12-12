/*
 @ 0xCCCCCCCC
*/

#include <conio.h>

void HelloConcurrencyWorld();

void SafeWaitThreadComplete();
void UseScopedThread();

void TestParallelAccumulate();

void TestHierarchicalMutex();

int main()
{
    TestHierarchicalMutex();
    _getch();
    return 0;
}
