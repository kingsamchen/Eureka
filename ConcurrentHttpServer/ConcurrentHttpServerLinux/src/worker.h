/* 
 @ 0xCCCCCCCC
*/

#ifndef WORKER_H_
#define WORKER_H_

#include "kbase/basic_macros.h"

class Worker {
public:
    ~Worker() = default;

    DISALLOW_COPY(Worker);

    DISALLOW_MOVE(Worker);

private:
   int epfd_; 
};

#endif  // WORKER_H_

