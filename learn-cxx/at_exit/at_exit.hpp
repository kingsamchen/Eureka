#pragma once

#include <cstddef>
#include <functional>
#include <mutex>
#include <optional>
#include <vector>

#include <boost/thread/null_mutex.hpp>

namespace base {
namespace detail {

template<bool ThreadSafe>
class BaseExitManager {
public:
    using Task = std::move_only_function<void()>;

    BaseExitManager() {
        constexpr std::size_t kInitCap = 8;
        tasks_.reserve(kInitCap);
    }

    ~BaseExitManager() {
        processTasks();
    }

    BaseExitManager(const BaseExitManager&) = delete;
    BaseExitManager(BaseExitManager&&) = delete;
    BaseExitManager& operator=(const BaseExitManager&) = delete;
    BaseExitManager& operator=(BaseExitManager&&) = delete;

    // The `task` should not register another task.
    void registerTask(Task task) {
        assert(task);
        std::scoped_lock lock(mtx_);
#ifndef NDEBUG
        assert(!processing_);
#endif
        tasks_.push_back(std::move(task));
    }

    // Executes the tasks registered previously in LIFO order.
    void processTasks() noexcept {
        std::vector<Task> tasks;

        {
            std::scoped_lock lock(mtx_);
            tasks.swap(tasks_);
#ifndef NDEBUG
            processing_ = true;
#endif
        }

        while (!tasks.empty()) {
            auto& task = tasks.back();
            task();
            tasks.pop_back();
        }

#ifndef NDEBUG
        std::scoped_lock lock(mtx_);
        assert(tasks_.empty());
        processing_ = false;
#endif
    }

private:
    using Mutex = std::conditional_t<ThreadSafe, std::mutex, boost::null_mutex>;

    [[no_unique_address]] Mutex mtx_;
    std::vector<Task> tasks_;
#ifndef NDEBUG
    bool processing_{false};
#endif
};

} // namespace detail

class AtExitManager : private detail::BaseExitManager<false> {
    using Base = detail::BaseExitManager<false>;

public:
    using Base::processTasks;
    using Base::registerTask;
    using Base::Task;

    class ThreadSafe;
    class Global;
};

class AtExitManager::ThreadSafe : private detail::BaseExitManager<true> {
    using Base = detail::BaseExitManager<true>;

public:
    using Base::processTasks;
    using Base::registerTask;
    using Base::Task;
};

class AtExitManager::Global {
public:
    Global() {
        assert(g_instance_ == nullptr);
        exit_manager_impl_.emplace();
        g_instance_ = &(*exit_manager_impl_);
    }

    ~Global() {
        assert(g_instance_ == &(*exit_manager_impl_));
        exit_manager_impl_.reset();
        g_instance_ = nullptr;
    }

    Global(const Global&) = delete;
    Global(Global&&) = delete;
    Global& operator=(const Global&) = delete;
    Global& operator=(Global&&) = delete;

    static void registerTask(Task task) {
        assert(g_instance_ != nullptr);
        g_instance_->registerTask(std::move(task));
    }

private:
    using Impl = detail::BaseExitManager<true>;

    std::optional<Impl> exit_manager_impl_;
    inline static Impl* g_instance_{nullptr};
};

} // namespace base
