/*
 @ 0xCCCCCCCC
*/

#include "kbase/basic_macros.h"
#include "kbase/logging.h"

#include "conn_pool/pool.h"

struct RedisConn {
    RedisConn()
    {
        LOG(INFO) << "RedisConn ctor";
    }

    DISALLOW_COPY(RedisConn);

    DEFAULT_MOVE(RedisConn);

    void Set(const std::string& key, const std::string& value)
    {
        FORCE_AS_MEMBER_FUNCTION();
        LOG(INFO) << "SET " << key << " " << value;
    }

    std::string Get(const std::string& key)
    {
        FORCE_AS_MEMBER_FUNCTION();
        LOG(INFO) << "GET " << key;
        return {};
    }
};

struct RedisConnTraits {
    using ConnType = RedisConn;

    static ConnType NewConn()
    {
        return RedisConn();
    }

    static void BeforeReturnToPool(ConnType& conn)
    {
        UNUSED_VAR(conn);
        // Handle unfinished/pending pipeline...
    }
};

int main()
{
    kbase::LoggingSettings settings;
    settings.logging_destination = kbase::LogToSystemDebugLog;
    kbase::ConfigureLoggingSettings(settings);

    auto redis_pool = ConnPool<RedisConnTraits>::Make(100, 10, std::chrono::seconds(10));

    {
        auto conn = redis_pool->Get();

        conn->Set("abc", "daf");
        IGNORE_RESULT(conn->Get("abc"));

        LOG(INFO) << "Done using conn";
    }

    return 0;
}
