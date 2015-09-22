/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef THREAD_SAFE_OBSERVER_OBSERVER_LIST_H_
#define THREAD_SAFE_OBSERVER_OBSERVER_LIST_H_

#include <algorithm>
#include <memory>
#include <mutex>
#include <vector>

template<typename ObserverT>
class ObserverList {
public:
    ObserverList() = default;

    ObserverList(const ObserverList&) = delete;

    // TODO: move-ctor

    ~ObserverList() = default;

    ObserverList& operator=(const ObserverList&) = delete;

    // TODO: move-assignment

    void Add(std::weak_ptr<ObserverT> observer)
    {
        std::lock_guard<std::mutex> lock(mutex_);
#if defined(_DEBUG)
        if (Has(observer)) {
            throw std::logic_error("Observer can only be added once!");
        }
#endif
        list_.push_back(observer);
    }

    void Remove(std::weak_ptr<ObserverT> observer)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = std::find_if(list_.cbegin(), list_.cend(), [&observer](const auto& ptr) {
            return !observer.owner_before(ptr) && ptr.owner_before(observer);
        });

        if (it != list_.cend()) {
            list_.erase(it);
        }
    }

private:
    bool Has(const std::weak_ptr<ObserverT>& observer)
    {
        auto it = std::find_if(list_.cbegin(), list_.cend(), [&observer](const auto& ptr) {
            return !observer.owner_before(ptr) && ptr.owner_before(observer);
        });

        return it != list_.cend();
    }

private:
    using ListType = std::vector<std::weak_ptr<ObserverT>>;
    ListType list_;
    std::mutex mutex_;
};

#endif // THREAD_SAFE_OBSERVER_OBSERVER_LIST_H_
