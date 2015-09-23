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
#include <vector>

// A helper class for implementing observer pattern.
// Note that this class itself is not thread-safe, its containing observable should take care
// of synchronization between threads.
// See sample code for its usage.
template<typename ObserverT>
class ObserverList {
private:
    using ListType = std::vector<std::weak_ptr<ObserverT>>;

public:
    ObserverList() = default;

    ObserverList(const ObserverList&) = delete;

    ObserverList(ObserverList&& other)
        : list_(std::move(other.list_))
    {}

    ~ObserverList() = default;

    ObserverList& operator=(const ObserverList&) = delete;

    ObserverList& operator=(ObserverList&& rhs)
    {
        if (this != &rhs) {
            list_ = std::move(rhs.list_);
        }

        return *this;
    }

    void Add(std::weak_ptr<ObserverT> observer)
    {
#if !defined(NDEBUG)
        bool has_already = Find(observer) != list_.cend();
        if (has_already) {
            throw std::logic_error("Observer can only be added once!");
        }
#endif
        list_.push_back(observer);
    }

    void Remove(std::weak_ptr<ObserverT> observer)
    {
        auto it = Find(observer);
        if (it != list_.cend()) {
            list_.erase(it);
        }
    }

    void Compact()
    {
        list_.erase(std::remove_if(list_.begin(), list_.end(), [](const auto& ptr) {
            return ptr.expired();
        }), list_.end());
    }

    // The containing observable must keep the ObserverList object alive when iterating
    // its observers.
    class Enumerator {
    public:
        explicit Enumerator(ObserverList& observer_list)
            : iter_(observer_list.list_.cbegin()), end_(observer_list.list_.cend())
        {}

        // Returns a shared_ptr object that refers to the next alive observer; or
        // returns an empty shared_ptr if there is no alive observer.
        std::shared_ptr<ObserverT> Next()
        {
            while (iter_ != end_) {
                auto owned = iter_->lock();
                ++iter_;
                if (owned) {
                    return owned;
                }
            }

            return std::shared_ptr<ObserverT>();
        }

    private:
        using iterator = typename ListType::const_iterator;
        iterator iter_;
        iterator end_;
    };

private:
    auto Find(const std::weak_ptr<ObserverT>& observer)
    {
        return std::find_if(list_.cbegin(), list_.cend(), [&observer](const auto& ptr) {
            return !observer.owner_before(ptr) && !ptr.owner_before(observer);
        });
    }

private:
    ListType list_;
};

#define FOR_EACH_OBSERVER(ObserverType, List, Fn)           \
    do {                                                    \
        decltype(List)::Enumerator enumerator(List);        \
        std::shared_ptr<ObserverType> observer;             \
        while (observer = enumerator.Next()) {              \
            observer->Fn;                                   \
        }                                                   \
        List.Compact();                                     \
    } while(false);

#endif // THREAD_SAFE_OBSERVER_OBSERVER_LIST_H_
