/*
 @ 0xCCCCCCCC
*/

#pragma once

#include <mutex>

#include "kbase/basic_macros.h"
#include "kbase/error_exception_util.h"

// Don't forget access to a mutex instance is from multiple threads, and therefore,
// `previous_hierarchy_value_` is a shared member and should be accessed under protection.
class HierarchicalMutex {
public:
    explicit HierarchicalMutex(unsigned int hierarchy_value)
        : hierarchy_value_(hierarchy_value), previous_hierarchy_value_(0)
    {}

    void lock()
    {
        CheckHierarchyViolation();

        mutex_.lock();

        UpdateThreadHierarchyValue();
    }

    void unlock()
    {
        current_thread_hierachy_value = previous_hierarchy_value_;
        mutex_.unlock();
    }

    bool try_lock()
    {
        CheckHierarchyViolation();

        if (mutex_.try_lock()) {
            UpdateThreadHierarchyValue();
            return true;
        }

        return false;
    }

    DISALLOW_COPY(HierarchicalMutex);

    DISALLOW_MOVE(HierarchicalMutex);

private:
    void CheckHierarchyViolation() const
    {
        ENSURE(THROW, current_thread_hierachy_value > hierarchy_value_)(hierarchy_value_)
            (current_thread_hierachy_value).Require();
    }

    void UpdateThreadHierarchyValue()
    {
        previous_hierarchy_value_ = current_thread_hierachy_value;
        current_thread_hierachy_value = hierarchy_value_;
    }

private:
    const unsigned int hierarchy_value_;
    unsigned int previous_hierarchy_value_;
    thread_local static unsigned int current_thread_hierachy_value;
    std::mutex mutex_;
};

thread_local unsigned int HierarchicalMutex::current_thread_hierachy_value =
    std::numeric_limits<unsigned int>::max();
