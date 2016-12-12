#ifndef DOWNLOADER_URL_DOWNLOADER_H_
#define DOWNLOADER_URL_DOWNLOADER_H_

#include <memory>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "net/base/io_buffer.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_context.h"
#include "url/gurl.h"

namespace bililive {

class URLDownloader : public net::URLRequest::Delegate {
public:
    class CompleteCallback {
    public:
        virtual ~CompleteCallback() {}

        virtual void OnDownloadSuccess() = 0;

        virtual void OnDownloadStopped() = 0;

        virtual void OnDownloadFailure() = 0;
    };

    URLDownloader(const GURL& url, const base::FilePath& save_path, CompleteCallback* callback);

    ~URLDownloader() = default;

    void Start();

    void Stop();

private:
    DISALLOW_COPY_AND_ASSIGN(URLDownloader);

    void OnResponseStarted(net::URLRequest* request) override;

    void OnReadCompleted(net::URLRequest* request, int bytes_read) override;

    void SaveReceivedChunk(int bytes_received, bool force_write_to_disk);

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
};

}   // namespace bililive

#endif  // DOWNLOADER_URL_DOWNLOADER_H_
