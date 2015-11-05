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

template<typename Func>
class SlotImpl {

};

template<typename Func, typename... Args>
class SignalImpl {
private:
    using SlotImpl = SlotImpl<Func>;
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
        // TODO: would modify slots_
    }

    void Invoke(Args... args)
    {
        // TODO: call on each slot
        __debugbreak();
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
