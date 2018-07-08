/*
 @ 0xCCCCCCCC
*/

#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <sys/types.h>

#include "kbase/basic_macros.h"

namespace network {

class EventLoop {
public:
    EventLoop();

    ~EventLoop();

    DISALLOW_COPY(EventLoop);

    DISALLOW_MOVE(EventLoop);

    void Run();

    static EventLoop* current();

private:
    bool is_running_;
    pid_t associated_thread_id_;
};

}   // namespace network

#endif //EVENT_LOOP_H_
