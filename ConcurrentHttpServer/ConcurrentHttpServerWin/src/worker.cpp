/*
 @ 0xCCCCCCCC
*/

#include "worker.h"

#include "kbase/error_exception_util.h"
#include "kbase/logging.h"

#include "iocp_utils.h"
#include "tcp_connection.h"

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

        auto conn = static_cast<TcpConnection*>(ov);

        if (!status) {
            kbase::LastError err;
            LOG(WARNING) << err;
            // TODO: handle error.
        } else if (completion_key == utils::CompletionKeyAccept) {
            conn->OnIOComplete(static_cast<int64_t>(bytes_transferred));
            // TODO: issue another accept request
        } else if (completion_key == utils::CompletionKeyIO) {
            conn->OnIOComplete(static_cast<int64_t>(bytes_transferred));
        } else if (completion_key == utils::CompletionKeyShutdown) {
            break;
        } else {
            LOG(WARNING) << "Unknown CompletionKey: " << completion_key;
            break;
        }
    }
}
