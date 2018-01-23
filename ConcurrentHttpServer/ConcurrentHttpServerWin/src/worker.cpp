/*
 @ 0xCCCCCCCC
*/

#include "worker.h"

#include "kbase/error_exception_util.h"
#include "kbase/logging.h"

#include "iocp_utils.h"

Worker::Worker(HANDLE io_port, SOCKET listener)
    : io_port_(io_port), listener_(listener)
{}

void Worker::operator()() const
{
    WorkProc();
}

void Worker::WorkProc() const
{
    while (true) {
        DWORD bytes_transferred = 0;
        ULONG_PTR completion_key = 0;
        LPOVERLAPPED ov = nullptr;

        BOOL status = GetQueuedCompletionStatus(io_port_, &bytes_transferred, &completion_key, &ov,
                                                INFINITE);
        if (!status) {
            kbase::LastError err;
            LOG(WARNING) << err;
            // TODO: handle error.
        } else if (completion_key == utils::CompletionKeyAccept) {
            // TODO: handle accept new connection
            // TODO: issue another accept request
        } else if (completion_key == utils::CompletionKeyIO) {
            // TODO: handle incoming requests
        } else if (completion_key == utils::CompletionKeyShutdown) {
            break;
        } else {
            LOG(WARNING) << "Unknown CompletionKey: " << completion_key;
            break;
        }
    }
}
