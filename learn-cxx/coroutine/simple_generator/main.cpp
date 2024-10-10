#include <concepts>
#include <coroutine>
#include <exception>
#include <iostream>
#include <utility>

template<typename T>
class generator {
public:
    // coroutine handle is like a ptr.
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    explicit generator(handle_type handle)
        : handle_(handle) {}

    generator(generator&& other) noexcept
        : handle_(std::exchange(other.handle_, handle_type{})) {}

    // Leave as-is for move-assignment?

    // Manage coroutine frame on our own.
    ~generator() {
        if (handle_) {
            handle_.destroy();
        }
    }

    generator(const generator&) = delete;
    generator& operator=(const generator&) = delete;

    explicit operator bool() {
        fill();
        return !handle_.done();
    }

    T operator()() {
        fill();
        promise_value_set_ = false;
        return std::move_if_noexcept(handle_.promise().value_);
    }

    //
    // promise_type
    //

    struct promise_type {
        T value_;
        std::exception_ptr exception_;

        //
        // Six required functions of a promise.
        //

        generator get_return_object() {
            // promise -> coroutine ahndle
            return generator(handle_type::from_promise(*this));
        }

        std::suspend_always initial_suspend() noexcept {
            return std::suspend_always{};
        }

        // We want to manually cleanup resource, so we suspend the coroutine at final suspend point.
        std::suspend_always final_suspend() noexcept {
            return std::suspend_always{};
        }

        void unhandled_exception() {
            exception_ = std::current_exception();
        }

        template<typename From>
        requires std::convertible_to<From, T>
        std::suspend_always yield_value(From&& from) {
            value_ = std::forward<From>(from);
            return std::suspend_always{};
        }

        void return_void() {}
    };

private:
    void fill() {
        if (!promise_value_set_) {
            // resume execution of coroutine.
            handle_();

            // Propagate exception if in active.
            if (handle_.promise().exception_) {
                std::rethrow_exception(handle_.promise().exception_);
            }

            promise_value_set_ = true;
        }
    }

private:
    handle_type handle_;
    bool promise_value_set_{false};
};

generator<int> fib(int n) {
    if (n == 0) {
        throw std::invalid_argument("n >= 1");
    }

    if (n > 94) {
        throw std::invalid_argument("too big");
    }

    co_yield 1;

    if (n == 1) {
        co_return;
    }

    co_yield 1;

    if (n == 2) {
        co_return;
    }

    for (int a = 1, b = 1, i = 3; i <= n; ++i) {
        auto s = a + b;
        co_yield s;
        a = b;
        b = s;
    }
}

int main() {
    try {
        auto gen = fib(10);
        for (int i = 1; gen; ++i) {
            std::cout << "fib(" << i << ")=" << gen() << "\n";
        }
    } catch (const std::exception& ex) {
        std::cerr << "Unhandled: " << ex.what();
        return 1;
    }
}

