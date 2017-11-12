/*
 @ 0xCCCCCCCC
*/

#include <conio.h>

void BasicMemoryMappedFile();

void TestBroadcastOnEvent();
void TestMTQueue();
void TestAPC();

void TestAnonymousPipe();
void TestNamedPipe();

void TestSocketMessage();

void TestOverlappedIO();

int main()
{
    TestOverlappedIO();
    _getch();
    return 0;
}
