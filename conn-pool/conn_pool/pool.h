/*
 @ 0xCCCCCCCC
*/

#ifndef CONN_POOL_POOL_H_
#define CONN_POOL_POOL_H_

#include <deque>
#include <memory>
#include <type_traits>

#include "kbase/basic_macros.h"

template<typename Traits>
class ConnPool : public std::enable_shared_from_this<ConnPool<Traits>> {
private:
    class Passkey {
        explicit Passkey() = default;
        friend ConnPool;
    };

public:
    using RawConnType = typename Traits::ConnType;

    static_assert(std::is_move_constructible_v<RawConnType>,
                  "Traits::ConnType must be move-constructible");

    class Conn {
    public:
        using Pool = ConnPool<Traits>;

        Conn(RawConnType raw_conn, const std::shared_ptr<Pool>& pool)
            : raw_conn_(std::move(raw_conn)),
              bound_pool_(pool)
        {}

        ~Conn()
        {
            auto pool = bound_pool_.lock();
            if (pool) {
                pool->Put(std::move(raw_conn_));
            }
        }

        DISALLOW_COPY(Conn);

        DEFAULT_MOVE(Conn);

    private:
        RawConnType raw_conn_;
        std::weak_ptr<Pool> bound_pool_;
    };

    static_assert(std::is_move_constructible_v<Conn> && std::is_move_assignable_v<Conn>,
                  "Conn must be both move-constructible and move-assignable");

    ConnPool(Passkey, size_t capacity, size_t max_reused)
        : capacity_(capacity),
          size_(0),
          max_idle_(max_reused)
    {}

    ~ConnPool() = default ;

    DISALLOW_COPY(ConnPool);

    DISALLOW_MOVE(ConnPool);

    static std::shared_ptr<ConnPool> Make(size_t capacity, size_t max_reused)
    {
        return std::make_shared<ConnPool>(Passkey{}, capacity, max_reused);
    }

    // TODO: Must be thread-safe
    Conn Get()
    {
        return Conn(Traits::NewConn(), this->shared_from_this());
    }

    // TODO: Must be thread-safe
    void Put(RawConnType raw_conn)
    {}

private:
    size_t capacity_;
    size_t size_;
    size_t max_idle_;
    std::deque<RawConnType> idle_list_;
};

#endif  // CONN_POOL_POOL_H_
