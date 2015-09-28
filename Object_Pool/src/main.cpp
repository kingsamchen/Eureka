/*
 @ 0xCCCCCCCC
*/

#include "kbase\at_exit_manager.h"

#include "object_pool.h"

int main()
{
    kbase::AtExitManager exit_manager;
    auto title = ObjectFactory<std::string>::GetInstance()->Get("title");
    *title = "hello, world";
    {
        auto pn = ObjectFactory<std::string>::GetInstance()->Get("name");
        *pn = "kingsley";
    }

    return 0;
}