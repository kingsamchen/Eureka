#include "downloader/url_downloader.h"

#include "net/url_request/url_request_context_builder.h"
#include "net/url_request/url_request_status.h"

namespace {

const int kIOBufSize = 16 * 1024;   // 16KB
const size_t kDataCacheSize = 4 * 1024 * 1024;  // 4MB

// If a request has failed, it must end up with the status being either failed, or canceled.
bool InvalidFailureStatus(net::URLRequestStatus::Status status)
{
    return status != net::URLRequestStatus::FAILED &&
           status != net::URLRequestStatus::CANCELED;
}

}   // namespace

namespace bililive {

URLDownloader::URLDownloader(const GURL& url, const base::FilePath& save_path, CompleteCallback* callback)
    : url_(url), save_path_(save_path), complete_callback_(callback), buf_(new net::IOBuffer(kIOBufSize))
{
    net::URLRequestContextBuilder context_builder;
    request_context_.reset(context_builder.Build());

    request_ = std::make_unique<net::URLRequest>(url, this, request_context_.get());
    request_->StopCaching();
}

void URLDownloader::Start()
{
    request_->Start();
}

void URLDownloader::Stop()
{}

void URLDownloader::OnResponseStarted(net::URLRequest* request)
{
    DCHECK(!request->status().is_io_pending());

    // TODO: Get full request headers? we need to resume download from where last ends.

    if (!request->status().is_success()) {
        auto status = request->status().status();
        DCHECK(!InvalidFailureStatus(status));
        LOG_IF(ERROR, InvalidFailureStatus(status)) << "Request failed with invalid status: " << status;
        if (status == net::URLRequestStatus::CANCELED) {
            complete_callback_->OnDownloadStopped();
        } else {
            complete_callback_->OnDownloadFailure();
        }

        return;
    }

    int bytes_read = 0;
    if (request->Read(buf_.get(), kIOBufSize, &bytes_read)) {
        OnReadCompleted(request, bytes_read);
    } else {
        // TODO: On download complete.
    }
}

void URLDownloader::OnReadCompleted(net::URLRequest* request, int bytes_read)
{
    DCHECK(!request->status().is_io_pending());
}

}   // namespace bililive