/*
 @ 0xCCCCCCCC
*/

#include <conio.h>

void HelloConcurrencyWorld();

void SafeWaitThreadComplete();
void UseScopedThread();

void TestParallelAccumulate();

void TestHierarchicalMutex();

void TestPackagedTasks();
void TestPromise();

void TestRelaxed();

int main()
{
    TestRelaxed();
    _getch();
    return 0;
}
