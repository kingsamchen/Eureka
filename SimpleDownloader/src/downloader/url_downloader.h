#ifndef DOWNLOADER_URL_DOWNLOADER_H_
#define DOWNLOADER_URL_DOWNLOADER_H_

#include <memory>

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

private:
    GURL url_;
    base::FilePath save_path_;
    // Non-owning. The provider of the callback should take care of the lifetime for callback.
    CompleteCallback* complete_callback_;
    scoped_refptr<net::IOBuffer> buf_;
    std::unique_ptr<net::URLRequestContext> request_context_;
    std::unique_ptr<net::URLRequest> request_;
};

}   // namespace bililive

#endif  // DOWNLOADER_URL_DOWNLOADER_H_
