#ifndef DOWNLOADER_URL_DOWNLOADER_H_
#define DOWNLOADER_URL_DOWNLOADER_H_

#include <memory>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/threading/thread_checker.h"
#include "net/base/io_buffer.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_context.h"
#include "url/gurl.h"

namespace downloader {

// The user of URLDownloader must ensure that the construction and operations of an URLDownloader
// instance are performed on a same thread.
class URLDownloader : public net::URLRequest::Delegate {
public:
    class CompleteCallback {
    public:
        virtual ~CompleteCallback() {}

        virtual void OnDownloadSuccess() = 0;

        virtual void OnDownloadFailure() = 0;
    };

    // The caller of this constructor must make sure:
    // - `save_path` is writable;
    // - the object `callback` refers to is alive during the entire lifecycle of URLDownloader, since
    //   the class itself doesn't acquire the ownership for the `callback`.
    URLDownloader(const GURL& url, const base::FilePath& save_path, CompleteCallback* callback);

    ~URLDownloader() = default;

    // Starts or resumes the download.
    void Start();

    // Stops the download.
    void Stop();

private:
    void OnResponseStarted(net::URLRequest* request) override;

    void OnReadCompleted(net::URLRequest* request, int bytes_read) override;

    // Saves received network data (stored in `buf_`) to disk write cache, and writes to disk
    // if necessary.
    // It is legal to call this function with 0 passed in as `bytes_received`, when you want to
    // force flushing cache to disk.
    void SaveReceivedChunk(int bytes_received, bool force_write_to_disk);

    DISALLOW_COPY_AND_ASSIGN(URLDownloader);

private:
    GURL url_;
    base::FilePath save_path_;
    base::FilePath tmp_save_path_;
    CompleteCallback* complete_callback_;
    size_t downloaded_bytes_;
    std::vector<char> disk_write_cache_;
    scoped_refptr<net::IOBuffer> buf_;
    std::unique_ptr<net::URLRequestContext> request_context_;
    std::unique_ptr<net::URLRequest> request_;
    base::ThreadChecker thread_checker_;
};

}   // namespace bililive

#endif  // DOWNLOADER_URL_DOWNLOADER_H_