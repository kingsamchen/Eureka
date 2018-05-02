/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <thread>

void Hello()
{
    std::cout << "Hello Concurrency World!" << std::endl;
}

void HelloConcurrencyWorld()
{
    std::thread th(Hello);
    th.join();
}