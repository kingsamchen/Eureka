/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <thread>

#include "active_thread.h"

int main()
{
    ActiveThread active_handler;
    active_handler.PostTask([] { std::cout << "active handler starts running\n"; });
    for (int i = 0; i < 10; ++i) {
        active_handler.PostTask([i] { std::cout << "issuing in the " << i << "th round\n"; });
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cin.get();
    return 0;
}
