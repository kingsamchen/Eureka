/*
 @ 0xCCCCCCCC
*/

#include <iostream>
#include <mutex>

#include "observer_list.h"

namespace {

class Widget {
public:
    class Observer : public std::enable_shared_from_this<Observer> {
    public:
        virtual ~Observer() = default;
        virtual void PositionChanged(int x, int y) = 0;
    };

    void AddObserver(const std::weak_ptr<Observer>& observer)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        observers_.Add(observer);
    }

    void RemoveObserver(const std::weak_ptr<Observer>& observer)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        observers_.Remove(observer);
    }

    // Don't call `AddObserver` or `RemoveObserver` during notifying;
    // Otherwise it will result in deadlock since the lock is not reentranble.
    void NotifyFoo(int x, int y)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        FOR_EACH_OBSERVER(Observer, observers_, PositionChanged(x, y));
    }

private:
    ObserverList<Observer> observers_;
    std::mutex mutex_;
};

class PositionWatcher : public Widget::Observer {
public:
    void PositionChanged(int x, int y) override
    {
        std::cout << "new position (" << x << ", " << y << ")" << std::endl;
    }
};

}

int main()
{
    Widget my_widget;
    {
        auto watcher = std::make_shared<PositionWatcher>();
        my_widget.AddObserver(watcher);
        my_widget.NotifyFoo(20, 10);
    }
    my_widget.NotifyFoo(10, 20);

    return 0;
}

