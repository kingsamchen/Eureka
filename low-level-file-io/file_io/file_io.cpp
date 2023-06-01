//
// Kingsley Chen
//

#include "file_io/file_io.h"

#include <filesystem>
#include <system_error>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "esl/unique_handle.h"

namespace llio {
namespace {

inline std::size_t get_file_size(int fd) {
    struct stat64 file_stat {};
    const int ret = ::fstat64(fd, &file_stat);
    return ret == 0 ? static_cast<std::size_t>(file_stat.st_size) : 0;
}

} // namespace

void write_to_file(const std::string& path, std::string_view content, std::error_code& ec) {
    ec.clear();

    constexpr auto flags = O_CREAT | O_WRONLY | O_TRUNC | O_CLOEXEC;
    constexpr mode_t mode = 0666; // rw-rw-rw-
    const auto fd = esl::wrap_unique_fd(::open(path.c_str(), flags, mode));
    if (!fd) {
        ec = std::error_code(errno, std::system_category());
        return;
    }

    if (content.empty()) {
        return;
    }

    // Partial writes to the disk file may still happen due to:
    //  - insufficient space on the disk
    //  - interrupted by signal after at least one byte has been writte
    //  - one write operation on Linux, even for 64-bit systems, will transfer
    //    at most SSIZE_MAX even on 64-bit
    auto data = content.data();
    std::size_t size = content.size();
    for (; size != 0;) {
        const ssize_t bytes = ::write(fd.get(), data, size);
        if (bytes == -1) {
            if (errno == EINTR) {
                continue;
            }
            ec = std::error_code(errno, std::system_category());
            return;
        }

        data += bytes;
        size -= static_cast<std::size_t>(bytes);
    }
}

void write_to_file(const std::string& path, std::string_view content) {
    std::error_code ec;
    write_to_file(path, content, ec);
    if (ec) {
        throw std::filesystem::filesystem_error(path, ec);
    }
}

void read_from_file(const std::string& path, std::string& content, std::error_code& ec) {
    content.clear();
    ec.clear();

    constexpr auto flags = O_RDONLY | O_CLOEXEC;
    const auto fd = esl::wrap_unique_fd(::open(path.c_str(), flags));
    if (!fd) {
        ec = std::error_code(errno, std::system_category());
        return;
    }

    const auto file_size = get_file_size(fd.get());

    // If we are able to obtain actual file size, try to read one extra byte to
    // reach EOF in one go, and trigger EOF in next lighter read, and also can
    // save one expensive buf expansion.
    // Otherwise, we read file in chunks. To avoid unnecessary memory overhead
    // for most files, use small chunk size in first read.
    constexpr auto initial_chunk_size = static_cast<std::size_t>(1024) * 4;
    constexpr auto default_chunk_size = static_cast<std::size_t>(1024) * 16;
    auto read_chunk_size = file_size > 0 ? file_size + 1 : initial_chunk_size;
    content.resize(read_chunk_size, '\0');
    std::size_t total_size_read = 0;
    while (true) {
        std::size_t actual_read = 0;

        // Try to full read in each round.
        // Until either read EOF or read in full if no error occurred.
        {
            auto buf = content.data() + total_size_read;
            auto count = read_chunk_size;
            ssize_t bytes = 0;
            do { // NOLINT(cppcoreguidelines-avoid-do-while)
                bytes = ::read(fd.get(), buf, count);
                if (bytes == -1) {
                    if (errno == EINTR) {
                        continue;
                    }
                    ec = std::error_code(errno, std::system_category());
                    return;
                }

                actual_read += static_cast<std::size_t>(bytes);
                buf += bytes;
                count -= static_cast<std::size_t>(bytes);
            } while (bytes != 0 && count != 0);
        }

        total_size_read += actual_read;
        if (total_size_read < content.size()) {
            break;
        }

        if (read_chunk_size == initial_chunk_size) {
            read_chunk_size = default_chunk_size;
        }

        content.resize(total_size_read + read_chunk_size);
    }

    content.resize(total_size_read);
}

void read_from_file(const std::string& path, std::string& content) {
    std::error_code ec;
    read_from_file(path, content, ec);
    if (ec) {
        throw std::filesystem::filesystem_error(path, ec);
    }
}

} // namespace llio
