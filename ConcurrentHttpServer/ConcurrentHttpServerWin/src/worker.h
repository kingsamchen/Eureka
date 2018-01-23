/*
 @ 0xCCCCCCCC
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef WORKER_H_
#define WORKER_H_

#include <windows.h>
#include <winsock2.h>

#include "kbase/basic_macros.h"

class Worker {
public:
    Worker(HANDLE io_port, SOCKET listener);

    ~Worker() = default;

    DEFAULT_COPY(Worker);

    DEFAULT_MOVE(Worker);

    void operator()() const;

private:
    void WorkProc() const;

private:
    HANDLE io_port_;
    SOCKET listener_;
};

#endif  // WORKER_H_
