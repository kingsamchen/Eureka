#include <iostream>

#include "conn_pool/pool.h"

struct RedisConn {
    RedisConn()
    {
        std::cout << "RedisConn ctor\n";
    }
};

struct RedisConnTraits {
    using ConnType = RedisConn;

    static ConnType NewConn()
    {
        return RedisConn();
    }
};

int main()
{
    auto redis_pool = ConnPool<RedisConnTraits>::Make(100, 10, std::chrono::seconds(10));

    {
        auto conn = redis_pool->Get();
        std::cout << "use with conn\n";
    }

    std::cout << "hello world" << std::endl;
    return 0;
}
