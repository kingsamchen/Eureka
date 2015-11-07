/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef EUREKA_SIGNALS_H_
#define EUREKA_SIGNALS_H_

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include "compiler_helper.h"

namespace internal {

template<typename Func, typename... Args>
class SignalImpl;

template<typename Func, typename... Args>
struct SlotImpl {
    using Source = SignalImpl<Func, Args...>;

    SlotImpl(const std::shared_ptr<Source>& signal_source, Func&& func)
        : fn(func), source(signal_source), weakly_bound(false)
    {}

    SlotImpl(const std::shared_ptr<Source>& signal_source, Func&& func,
             const std::shared_ptr<void>& object)
        : fn(func), source(signal_source), weakly_bound_object(object), weakly_bound(true)
    {}

    ~SlotImpl() = default;

    DISALLOW_COPY(SlotImpl);

    DISALLOW_MOVE(SlotImpl);

    Func fn;
    std::weak_ptr<Source> source;
    std::weak_ptr<void> weakly_bound_object;
    bool weakly_bound;
};

template<typename Func, typename... Args>
class SignalImpl {
private:
    using SlotImpl = SlotImpl<Func, Args...>;
    using SlotList = std::vector<std::shared_ptr<SlotImpl>>;

public:
    SignalImpl()
        : slots_(std::make_shared<SlotList>())
    {}

    ~SignalImpl() = default;

    DISALLOW_COPY(SignalImpl);

    DISALLOW_MOVE(SignalImpl);

    void AddSlot(const std::shared_ptr<SlotImpl>& slot)
    {
        // TODO: would modify slots
    }

    void Invoke(Args... args)
    {
        std::shared_ptr<SlotList> slots;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            slots = slots_;
        }

        for (const auto& slot : *slots) {
            if (slot->weakly_bound) {
                auto guard = slot->weakly_bound_object.lock();
                if (!guard) {
                    // The bound object that member function relies on has gone.
                    continue;
                }
            }

            slot->fn(std::forward<Args>(args)...);
        }
    }

private:
    std::shared_ptr<SlotList> slots_;
    std::mutex mutex_;
};

}   // namespace internal

template<typename... Args>
class Signal {
private:
    using Func = std::function<void(Args...)>;
    using SignalImpl = internal::SignalImpl<Func, Args...>;

public:
    Signal()
        : impl_(std::make_unique<SignalImpl>())
    {}

    ~Signal() = default;

    DISALLOW_COPY(Signal);

    DISALLOW_MOVE(Signal);

    void Connect(Func fn)
    {}

    void Emit(Args... args)
    {
        impl_->Invoke(std::forward<Args>(args)...);
    }

private:
    std::unique_ptr<SignalImpl> impl_;
};

#endif  // EUREKA_SIGNALS_H_
