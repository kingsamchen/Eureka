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
#include <vector>

#include "kbase/basic_macros.h"
#include "kbase/logging.h"

namespace chrono = std::chrono;

class PoolExhausted : public std::runtime_error {
public:
    explicit PoolExhausted(const std::string& what)
        : std::runtime_error(what)
    {}

    explicit PoolExhausted(const char* what)
        : std::runtime_error(what)
    {}
};

namespace internal {

template<typename Traits, typename=void>
struct is_pool_traits_valid : std::false_type {};

// Our pool traits requirements.
template<typename Traits>
struct is_pool_traits_valid<Traits, std::void_t<
    typename Traits::ConnType,
    std::enable_if_t<std::is_move_constructible_v<typename Traits::ConnType> &&
                     std::is_move_assignable_v<typename Traits::ConnType>>,
    std::enable_if_t<std::is_same_v<decltype(Traits::NewConn()), typename Traits::ConnType>>,
    decltype(Traits::BeforeReturnToPool(std::declval<typename Traits::ConnType &>()))
>> : std::true_type {};

}   // namespace internal

template<typename Traits>
class ConnPool : public std::enable_shared_from_this<ConnPool<Traits>> {
public:
    static_assert(internal::is_pool_traits_valid<Traits>::value, "illegal pool traits");

    using RawConnType = typename Traits::ConnType;

private:
    using clock = chrono::system_clock;

    // For passkey idiom.
    class Passkey {
        explicit Passkey() = default;
        friend ConnPool;
    };

    // Represents a connection that resides in idle list and wait for reuse
    struct IdleConn {
        RawConnType conn;
        clock::time_point created;
        clock::time_point last_active;

        IdleConn(RawConnType conn, clock::time_point created)
            : conn(std::move(conn)),
              created(created),
              last_active(clock::now())
        {}

        ~IdleConn() = default;

        DISALLOW_COPY(IdleConn);

        DEFAULT_MOVE(IdleConn);
    };

public:
    class Conn {
    public:
        using Pool = ConnPool<Traits>;
        using pointer = RawConnType*;
        using reference = RawConnType&;

        Conn(RawConnType raw_conn, const std::shared_ptr<Pool>& pool)
            : raw_conn_(std::move(raw_conn)),
              raw_conn_creation_(clock::now()),
              bound_pool_(pool)
        {}

        Conn(IdleConn&& idle, const std::shared_ptr<Pool>& pool)
            : raw_conn_(std::move(idle.conn)),
              raw_conn_creation_(idle.created),
              bound_pool_(pool)
        {}

        ~Conn()
        {
            try {
                //Traits::BeforeReturnToPool(raw_conn_);

                // The pool may have long gone...
                auto pool = bound_pool_.lock();
                if (pool) {
                    pool->Put(std::move(raw_conn_), raw_conn_creation_);
                }
            } catch (const std::exception& ex) {
                LOG(ERROR) << "Failed to return connection to pool; ex=" << ex.what();
            }
        }

        DISALLOW_COPY(Conn);

        DEFAULT_MOVE(Conn);

        pointer operator->() noexcept
        {
            return &raw_conn_;
        }

        reference operator*() noexcept
        {
            return raw_conn_;
        }

    private:
        RawConnType raw_conn_;
        clock::time_point raw_conn_creation_;
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

    // Takes a connection from the pool; create a new underlying connection in the fly,
    // if necessary.
    // This function throws on failure.
    // This function is thread-safe.
    Conn Get()
    {
        if (idle_timeout_.count() > 0) {
            RetireTimeoutIdleConnections();
        }

        {
            std::lock_guard lock(mtx_);

            // Try to reuse from idle connections first.
            // The pool's size keeps unchanged.
            if (!idle_list_.empty()) {
                Conn conn(std::move(idle_list_.back()), this->shared_from_this());
                idle_list_.pop_back();
                return conn;
            }

            if (size_ >= capacity_) {
                throw PoolExhausted("the pool is exhausted");
            }

            // We are about to create a new underlying connection.
            ++size_;
        }

        try {
            return Conn(Traits::NewConn(), this->shared_from_this());
        } catch (const std::exception& ex) {
            {
                std::lock_guard lock(mtx_);
                --size_;
            }

            LOG(ERROR) << "Failed to call Traits::NewConn; ex=" << ex.what();
            throw;
        }
    }

    // Puts a connection back into the pool.
    // Called by Conn during its destruction.
    // This function is thread-safe.
    void Put(RawConnType raw_conn, clock::time_point creation)
    {
        // Unluckily, we cannot assume IdleConn can have a default constructor.
        std::unique_ptr<IdleConn> to_retire;

        {
            std::lock_guard lock(mtx_);

            if (idle_list_.size() == max_idle_) {
                to_retire = std::make_unique<IdleConn>(std::move(idle_list_.front()));
                idle_list_.pop_front();
                --size_;
            }

            idle_list_.push_back(IdleConn(std::move(raw_conn), creation));
        }

        to_retire.reset();
    }

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
    void RetireTimeoutIdleConnections()
    {
        std::vector<IdleConn> to_retire;
        auto now = clock::now();

        {
            std::lock_guard lock(mtx_);
            while (!idle_list_.empty() && (now - idle_list_.front().last_active >= idle_timeout_)) {
                to_retire.push_back(std::move(idle_list_.front()));
                idle_list_.pop_front();
                --size_;
            }
        }
    }

private:
    const size_t capacity_;
    const size_t max_idle_;
    const chrono::microseconds idle_timeout_;

    mutable std::mutex mtx_;
    size_t size_;
    // Most inactive conn is at the head.
    std::deque<IdleConn> idle_list_;
};

#endif  // CONN_POOL_POOL_H_
