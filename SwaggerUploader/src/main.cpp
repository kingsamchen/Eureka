
#include <iostream>
#include <string>
#include <vector>

#include "base/platform_file.h"
#include "base/strings/stringprintf.h"
#include "base/md5.h"

std::string GenerateFileHash(base::PlatformFile file)
{
    const int kReadBufSize = 4 * 1024 * 1024;
    std::vector<char> buf(kReadBufSize);

    base::MD5Context md5_ctx;
    base::MD5Init(&md5_ctx);

    int64 total_bytes_read = 0;
    int bytes_read = 0;
    while ((bytes_read = base::ReadPlatformFile(file, total_bytes_read, buf.data(), kReadBufSize)) > 0) {
        base::MD5Update(&md5_ctx, base::StringPiece(buf.data(), bytes_read));
        total_bytes_read += bytes_read;
    }

    base::MD5Digest digest;
    base::MD5Final(&digest, &md5_ctx);

    return base::MD5DigestToBase16(digest);
}

int main()
{
    std::cout << base::StringPrintf("file\"; filename=\"%s", "demo1.mp4");
    return 0;
}