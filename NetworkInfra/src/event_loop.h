/*
 @ 0xCCCCCCCC
*/

#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <memory>

#include <sys/types.h>

#include "kbase/basic_macros.h"

#include "poller.h"

namespace network {

class Channel;

class EventLoop {
public:
    EventLoop();

    ~EventLoop();

    DISALLOW_COPY(EventLoop);

    DISALLOW_MOVE(EventLoop);

    void Run();

    void Quit();

    void UpdateChannel(Channel* channel);

    bool BelongsToCurrentThread() const;

    static EventLoop* current();

private:
    bool is_running_;
    pid_t owner_thread_id_;
    std::unique_ptr<Poller> poller_;
};

}   // namespace network

#endif //EVENT_LOOP_H_
