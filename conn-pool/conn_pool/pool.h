/*
 @ 0xCCCCCCCC
*/

#ifndef CONN_POOL_POOL_H_
#define CONN_POOL_POOL_H_

#include <chrono>
#include <deque>
#include <memory>
#include <mutex>
#include <type_traits>

#include "kbase/basic_macros.h"

namespace chrono = std::chrono;

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

    ConnPool(Passkey, size_t capacity, size_t max_reused, chrono::microseconds idle_timeout)
        : capacity_(capacity),
          max_idle_(max_reused),
          idle_timeout_(idle_timeout),
          size_(0)
    {}

    ~ConnPool() = default ;

    DISALLOW_COPY(ConnPool);

    DISALLOW_MOVE(ConnPool);

    // `capacity` indicates the maximum number of connections can in the pool; once this
    // threshold has been reached, no more new connection can be made.
    // `max_reused` indicates the maximum connections that can be reused by the pool.
    // A connection is saying idle if it is waiting for reuse.
    template<typename Rep, typename Period>
    static std::shared_ptr<ConnPool> Make(size_t capacity,
                                          size_t max_reused,
                                          const chrono::duration<Rep, Period>& idle_timeout)
    {
        return std::make_shared<ConnPool>(Passkey{}, capacity, max_reused,
            chrono::duration_cast<chrono::microseconds>(idle_timeout));
    }

    // TODO: Must be thread-safe
    Conn Get()
    {
        return Conn(Traits::NewConn(), this->shared_from_this());
    }

    // TODO: Must be thread-safe
    void Put(RawConnType)
    {}

    size_t capacity() const noexcept
    {
        return capacity_;
    }

    size_t max_reused() const noexcept
    {
        return max_idle_;
    }

    size_t size() const noexcept
    {
        std::lock_guard lock(mtx_);
        return size_;
    }

    size_t reused() const noexcept
    {
        std::lock_guard lock(mtx_);
        return idle_list_.size();
    }

private:
    const size_t capacity_;
    const size_t max_idle_;
    const chrono::microseconds idle_timeout_;

    mutable std::mutex mtx_;
    size_t size_;
    std::deque<RawConnType> idle_list_;
};

#endif  // CONN_POOL_POOL_H_
