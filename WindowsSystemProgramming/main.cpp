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
void TestExtendedIO();

int main()
{
    TestExtendedIO();
    _getch();
    return 0;
}
