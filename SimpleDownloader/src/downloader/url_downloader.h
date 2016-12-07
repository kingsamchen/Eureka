#ifndef DOWNLOADER_URL_DOWNLOADER_H_
#define DOWNLOADER_URL_DOWNLOADER_H_

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "url/gurl.h"

namespace bililive {

class URLDownloader {
public:
    URLDownloader(const GURL& url, const base::FilePath& save_path);

    ~URLDownloader() = default;

private:
    DISALLOW_COPY_AND_ASSIGN(URLDownloader);

private:
    GURL url_;
    base::FilePath save_path_;
};

}   // namespace bililive

#endif  // DOWNLOADER_URL_DOWNLOADER_H_
