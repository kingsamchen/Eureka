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

int main()
{
    TestPromise();
    _getch();
    return 0;
}
