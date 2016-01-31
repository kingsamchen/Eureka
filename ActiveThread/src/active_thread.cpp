/*
 @ 0xCCCCCCCC
*/

#include "active_thread.h"

ActiveThread::ActiveThread()
    : done_(false)
{
    thread_ = std::make_unique<std::thread>(&ActiveThread::Run, this);
}

void ActiveThread::Run()
{}
