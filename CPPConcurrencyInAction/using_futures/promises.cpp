/*
 @ 0xCCCCCCCC
*/

#include <future>
#include <iostream>
#include <random>
#include <thread>

void TestPromise()
{
    std::promise<int> server_key;

    std::thread worker([&server_key]() {
        std::random_device rd;
        int n = 0;
        for (int i = 0; i < 10; ++i) {
            n += rd();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        n ^= 0xCC;

        server_key.set_value(n);
    });

    std::cout << "The server key is ";

    auto key = server_key.get_future().get();

    worker.join();

    std::cout << key << std::endl;
}
