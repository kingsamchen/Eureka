#include "downloader/url_downloader.h"

#include <fstream>

#include "base/file_util.h"
#include "net/url_request/url_request_context_builder.h"
#include "net/url_request/url_request_status.h"

namespace {

const wchar_t kTmpFileExt[] = L"part";
const int kIOBufSize = 16 * 1024;   // 16KB
const size_t kDiskWriteThreshold = 4 * 1024 * 1024;  // 4MB

// If a request has failed, it must end up with the status being either failed, or canceled.
bool InvalidFailureStatus(net::URLRequestStatus::Status status)
{
    return status != net::URLRequestStatus::FAILED &&
           status != net::URLRequestStatus::CANCELED;
}

bool MarkDownloadedFileComplete(const base::FilePath& tmp_save_path, const base::FilePath& original_path)
{
    base::PlatformFileError error;
    bool rv = base::ReplaceFile(tmp_save_path, original_path, &error);
    if (!rv) {
        LOG(WARNING) << "Failed to rename downloaded file (" << tmp_save_path.AsUTF8Unsafe()
                     << ") to (" << original_path.AsUTF8Unsafe()
                     << "); Error: " << error;
        return false;
    }

    return true;
}

}   // namespace

namespace bililive {

URLDownloader::URLDownloader(const GURL& url, const base::FilePath& save_path, CompleteCallback* callback)
    : url_(url),
      save_path_(save_path),
      tmp_save_path_(save_path.AddExtension(kTmpFileExt)),
      complete_callback_(callback),
      downloaded_bytes_(0U),
      buf_(new net::IOBuffer(kIOBufSize))
{
    disk_write_cache_.reserve(kDiskWriteThreshold + kIOBufSize);

    net::URLRequestContextBuilder context_builder;
    request_context_.reset(context_builder.Build());

    request_ = std::make_unique<net::URLRequest>(url, this, request_context_.get());
}

void URLDownloader::Start()
{
    if (!base::PathExists(tmp_save_path_)) {
        std::ofstream tmp_file(tmp_save_path_.value());
        (void)tmp_file;
    }

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
            LOG(WARNING) << "Request failed";
            complete_callback_->OnDownloadFailure();
        }

        return;
    }

    // Now begin our initial read.
    int bytes_read = 0;
    if (request->Read(buf_.get(), kIOBufSize, &bytes_read)) {
        OnReadCompleted(request, bytes_read);
    } else if (!request->status().is_io_pending()) {
        LOG(WARNING) << "Read from response encountered error;"
                     << "status: " << request->status().status()
                     << "; error: " << request->status().error();
        complete_callback_->OnDownloadFailure();
    }
}

void URLDownloader::OnReadCompleted(net::URLRequest* request, int bytes_read)
{
    DCHECK(!request->status().is_io_pending());

    if (bytes_read > 0) {
        downloaded_bytes_ += static_cast<size_t>(bytes_read);
        SaveReceivedChunk(bytes_read, false);
    }

    if (request->status().is_success() && bytes_read > 0) {
        bytes_read = 0;
        while (request->Read(buf_.get(), kIOBufSize, &bytes_read)) {
            if (bytes_read == 0) {
                SaveReceivedChunk(0, true);
                if (MarkDownloadedFileComplete(tmp_save_path_, save_path_)) {
                    complete_callback_->OnDownloadSuccess();
                } else {
                    // Extremely rare, but possible.
                    complete_callback_->OnDownloadFailure();
                }

                return;
            }

            DCHECK(bytes_read > 0);
            downloaded_bytes_ += static_cast<size_t>(bytes_read);
            SaveReceivedChunk(bytes_read, false);
        }
    }

    if (!request->status().is_io_pending()) {
        LOG(WARNING) << "Read from response encountered error;"
                     << "status: " << request->status().status()
                     << "; error: " << request->status().error();
        complete_callback_->OnDownloadFailure();
    }
}

void URLDownloader::SaveReceivedChunk(int bytes_received, bool force_write_to_disk)
{
    DCHECK(bytes_received >= 0);

    auto data_begin = buf_->data();
    auto data_end = buf_->data() + bytes_received;
    disk_write_cache_.insert(disk_write_cache_.end(), data_begin, data_end);
    if (disk_write_cache_.size() >= kDiskWriteThreshold || force_write_to_disk) {
        file_util::AppendToFile(tmp_save_path_, disk_write_cache_.data(), disk_write_cache_.size());
        disk_write_cache_.clear();
    }
}

}   // namespace bililive