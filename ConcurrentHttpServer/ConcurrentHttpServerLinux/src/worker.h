/*
 @ 0xCCCCCCCC
*/

#ifndef WORKER_H_
#define WORKER_H_

#include "kbase/basic_macros.h"

class Worker {
public:
    explicit Worker(int epfd);

    ~Worker() = default;

    DEFAULT_COPY(Worker);

    DEFAULT_MOVE(Worker);

    void operator()() const;

private:
    void WorkProc() const;

private:
   int epfd_;
};

#endif  // WORKER_H_
